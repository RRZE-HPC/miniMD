/* ----------------------------------------------------------------------
   miniMD is a simple, parallel molecular dynamics (MD) code.   miniMD is
   an MD microapplication in the Mantevo project at Sandia National
   Laboratories ( http://www.mantevo.org ). The primary
   authors of miniMD are Steve Plimpton (sjplimp@sandia.gov) , Paul Crozier
   (pscrozi@sandia.gov) and Christian Trott (crtrott@sandia.gov).

   Copyright (2008) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This library is free software; you
   can redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License as published by the Free Software Foundation;
   either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this software; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA.  See also: http://www.gnu.org/licenses/lgpl.txt .

   For questions, contact Paul S. Crozier (pscrozi@sandia.gov) or
   Christian Trott (crtrott@sandia.gov).

   Please read the accompanying README and LICENSE files.
   ---------------------------------------------------------------------- */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <force_lj.h>
#include <likwid-markers.h>

#ifndef VECTORLENGTH
#define VECTORLENGTH 4
#endif

ForceLJ::ForceLJ()
{
    cutforce = 0.0;
    cutforcesq = 0.0;
    use_oldcompute = 0;
    reneigh = 1;
    style = FORCELJ;

    epsilon = 1.0;
    sigma6 = 1.0;
    sigma = 1.0;
}
ForceLJ::~ForceLJ() {}

void ForceLJ::setup()
{
    cutforcesq = cutforce * cutforce;
}


void ForceLJ::compute(Atom &atom, Neighbor &neighbor, Comm &comm, int me)
{
    eng_vdwl = 0;
    virial = 0;

    if(neighbor.halfneigh)
    {
        compute_halfneigh(atom, neighbor, me);
    } else {
        compute_fullneigh(atom, neighbor, me);
    }

    return;
}

void ForceLJ::compute_halfneigh(Atom &atom, Neighbor &neighbor, int me)
{
    int* neighs;
    const int nlocal = atom.nlocal;
    const int nall = atom.nlocal + atom.nghost;

    MMD_float* x = &atom.x[0][0];
    MMD_float* f = &atom.f[0][0];

    // clear force on own and ghost atoms
    for(int i = 0; i < nall; i++) {
        f[i * PAD + 0] = 0.0;
        f[i * PAD + 1] = 0.0;
        f[i * PAD + 2] = 0.0;
    }

    LIKWID_MARKER_START("halfneigh");
    // loop over all neighbors of my atoms
    // store force on both atoms i and j
    for(int i = 0; i < nlocal; i++) {
        neighs = &neighbor.neighbors[i * neighbor.maxneighs];
        const int numneighs = neighbor.numneigh[i];
        const MMD_float xtmp = x[i * PAD + 0];
        const MMD_float ytmp = x[i * PAD + 1];
        const MMD_float ztmp = x[i * PAD + 2];

        MMD_float fix = 0.0;
        MMD_float fiy = 0.0;
        MMD_float fiz = 0.0;

#ifdef USE_SIMD
#pragma simd reduction (+: fix,fiy,fiz)
#endif
        for(int k = 0; k < numneighs; k++) {
            const int j = neighs[k];
            const MMD_float delx = xtmp - x[j * PAD + 0];
            const MMD_float dely = ytmp - x[j * PAD + 1];
            const MMD_float delz = ztmp - x[j * PAD + 2];
            const MMD_float rsq = delx * delx + dely * dely + delz * delz;

            if(rsq < cutforcesq) {
                const MMD_float sr2 = 1.0 / rsq;
                const MMD_float sr6 = sr2 * sr2 * sr2 * sigma6;
                const MMD_float force = 48.0 * sr6 * (sr6 - 0.5) * sr2 * epsilon;

                fix += delx * force;
                fiy += dely * force;
                fiz += delz * force;

                f[j * PAD + 0] -= delx * force;
                f[j * PAD + 1] -= dely * force;
                f[j * PAD + 2] -= delz * force;
            }
        }

        f[i * PAD + 0] += fix;
        f[i * PAD + 1] += fiy;
        f[i * PAD + 2] += fiz;
    }
    LIKWID_MARKER_STOP("halfneigh");
}

//optimised version of compute
//  -MPI + OpenMP (using full neighborlists)
//  -gets rid of fj update (read/write to memory)
//  -use temporary variable for summing up fi
//  -enables vectorization by:
//    -get rid of 2d pointers
//    -use pragma simd to force vectorization of inner loop
void ForceLJ::compute_fullneigh(Atom &atom, Neighbor &neighbor, int me)
{
    int* neighs;
    const int nlocal = atom.nlocal;
    const int nall = atom.nlocal + atom.nghost;

    MMD_float* x = &atom.x[0][0];
    MMD_float* f = &atom.f[0][0];

    x = &atom.x[0][0];
    f = &atom.f[0][0];

    // clear force on own and ghost atoms
    for(int i = 0; i < nlocal; i++) {
        f[i * PAD + 0] = 0.0;
        f[i * PAD + 1] = 0.0;
        f[i * PAD + 2] = 0.0;
    }

    LIKWID_MARKER_START("fullneigh");
    // loop over all neighbors of my atoms
    // store force on atom i
    for(int i = 0; i < nlocal; i++) {
        neighs = &neighbor.neighbors[i * neighbor.maxneighs];
        const int numneighs = neighbor.numneigh[i];
        const MMD_float xtmp = x[i * PAD + 0];
        const MMD_float ytmp = x[i * PAD + 1];
        const MMD_float ztmp = x[i * PAD + 2];

        MMD_float fix = 0;
        MMD_float fiy = 0;
        MMD_float fiz = 0;

        for(int k = 0; k < numneighs; k++) {
            const int j = neighs[k];
            const MMD_float delx = xtmp - x[j * PAD + 0];
            const MMD_float dely = ytmp - x[j * PAD + 1];
            const MMD_float delz = ztmp - x[j * PAD + 2];
            const MMD_float rsq = delx * delx + dely * dely + delz * delz;

            if(rsq < cutforcesq) {
                const MMD_float sr2 = 1.0 / rsq;
                const MMD_float sr6 = sr2 * sr2 * sr2 * sigma6;
                const MMD_float force = 48.0 * sr6 * (sr6 - 0.5) * sr2 * epsilon;

                fix += delx * force;
                fiy += dely * force;
                fiz += delz * force;
            }
        }

        f[i * PAD + 0] += fix;
        f[i * PAD + 1] += fiy;
        f[i * PAD + 2] += fiz;
    }
    LIKWID_MARKER_STOP("fullneigh");
}
