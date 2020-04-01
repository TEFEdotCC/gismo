/** @file gsG1AuxiliaryEdgeMultiplePatches.h
 *
    @brief Reparametrize the Geometry for one Interface

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Farahat
*/

#pragma once

#include <gismo.h>
#include <gsCore/gsMultiPatch.h>
#include <gsG1Basis/gsG1AuxiliaryPatch.h>
# include <gsG1Basis/gsApproxG1BasisEdge.h>
# include <gsG1Basis/gsG1OptionList.h>

namespace gismo
{


class gsG1AuxiliaryEdgeMultiplePatches
{

public:

    // Constructor for one patch and it's boundary
    gsG1AuxiliaryEdgeMultiplePatches(const gsMultiPatch<> & sp, const size_t patchInd){
        auxGeom.push_back(gsG1AuxiliaryPatch(sp.patch(patchInd), patchInd));
    }

    // Constructor for two patches along the common interface
    gsG1AuxiliaryEdgeMultiplePatches(const gsMultiPatch<> & mp, const size_t firstPatch, const size_t secondPatch){
        auxGeom.push_back(gsG1AuxiliaryPatch(mp.patch(firstPatch), firstPatch));
        auxGeom.push_back(gsG1AuxiliaryPatch(mp.patch(secondPatch), secondPatch));
    }


    // Compute topology
    // After computeTopology() the patches will have the same patch-index as the position-index in auxGeom
    // EXAMPLE: global patch-index-order inside auxGeom: [2, 3, 4, 1, 0]
    //          in auxTop: 2->0, 3->1, 4->2, 1->3, 0->4
    gsMultiPatch<> computeAuxTopology(){
        gsMultiPatch<> auxTop;
        for(unsigned i = 0; i <  auxGeom.size(); i++){
            if(auxGeom[i].getPatch().orientation() == -1)
            {
                auxGeom[i].swapAxis();
                gsInfo << "Changed axis on patch: " << auxGeom[i].getGlobalPatchIndex() << "\n";
            }
            auxTop.addPatch(auxGeom[i].getPatch());
        }
        auxTop.computeTopology();
        return auxTop;
    }


    gsMultiPatch<> reparametrizeG1Interface(){
        gsMultiPatch<> repTop(this->computeAuxTopology());

        if(repTop.interfaces()[0].second().side().index() == 1 && repTop.interfaces()[0].first().side().index() == 3)
            return repTop;

        // Right patch along the interface. Patch 0 -> v coordinate. Edge west along interface
        switch (repTop.interfaces()[0].second().side().index())
        {
            case 1:
                gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << "\tLocal patch: " << repTop.interfaces()[0].second().patch << " not rotated\n";
                break;
            case 4: auxGeom[0].rotateParamClock();
                gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() <<"\tLocal patch: " << repTop.interfaces()[0].second().patch << " rotated clockwise\n";
                break;
            case 3: auxGeom[0].rotateParamAntiClock();
                gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() <<"\tLocal patch: " << repTop.interfaces()[0].second().patch << " rotated anticlockwise\n";
                break;
            case 2: auxGeom[0].rotateParamAntiClockTwice();
                gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() <<"\tLocal patch: " << repTop.interfaces()[0].second().patch << " rotated twice anticlockwise\n";
                break;
            default:
                break;
        }

        // Left patch along the interface. Patch 1 -> u coordinate. Edge south along interface
        switch (repTop.interfaces()[0].first().side().index())
        {
            case 3:
                gsInfo << "Global patch: " << auxGeom[1].getGlobalPatchIndex() <<"\tLocal patch: " << repTop.interfaces()[0].first().patch << " not rotated\n";
                break;
            case 4: auxGeom[1].rotateParamAntiClockTwice();
                gsInfo << "Global patch: " << auxGeom[1].getGlobalPatchIndex() <<"\tLocal patch: " << repTop.interfaces()[0].first().patch << " rotated twice anticlockwise\n";
                break;
            case 2: auxGeom[1].rotateParamAntiClock();
                gsInfo << "Global patch: " << auxGeom[1].getGlobalPatchIndex() <<"\tLocal patch: " << repTop.interfaces()[0].first().patch << " rotated anticlockwise\n";
                break;
            case 1: auxGeom[1].rotateParamClock();
                gsInfo << "Global patch: " << auxGeom[1].getGlobalPatchIndex() <<"\tLocal patch: " << repTop.interfaces()[0].first().patch << " rotated clockwise\n";
                break;
            default:
                break;
        }
       return this->computeAuxTopology();
    }


    gsMultiPatch<> reparametrizeG1Boundary(const int bInd){
        gsMultiPatch<> repTop(this->computeAuxTopology());
        if(auxGeom[0].getOrient())
        {
            switch (bInd)
            {
                case 3:
                    gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << " not rotated\n";
                    break;
                case 2:
                    auxGeom[0].rotateParamClock();
                    gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << " rotated clockwise\n";
                    break;
                case 4:
                    auxGeom[0].rotateParamAntiClockTwice();
                    gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << " rotated twice anticlockwise\n";
                    break;
                case 1:
                    auxGeom[0].rotateParamAntiClock();
                    gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << " rotated anticlockwise\n";
                    break;
            }
        }
        else
        {
            switch (bInd)
            {
                case 1:
                    gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << " not rotated\n";
                    break;
                case 4:
                    auxGeom[0].rotateParamClock();
                    gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << " rotated clockwise\n";
                    break;
                case 2:
                    auxGeom[0].rotateParamAntiClockTwice();
                    gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << " rotated twice anticlockwise\n";
                    break;
                case 3:
                    auxGeom[0].rotateParamAntiClock();
                    gsInfo << "Global patch: " << auxGeom[0].getGlobalPatchIndex() << " rotated anticlockwise\n";
                    break;
            }
        }
        return this->computeAuxTopology();
    }


    void computeG1InterfaceBasis(gsG1OptionList g1OptionList){

        gsMultiPatch<> mp_init;
        mp_init.addPatch(auxGeom[0].getPatch());// Right -> 0 = v along the interface
        mp_init.addPatch(auxGeom[1].getPatch()); // Left -> 1 = u along the interface

        gsMultiPatch<> test_mp(this->reparametrizeG1Interface()); // auxGeom contains now the reparametrized geometry
        gsMultiBasis<> test_mb(test_mp);

        gsApproxG1BasisEdge<real_t> g1BasisEdge_0(test_mp.patch(0), test_mb.basis(0), 1, false, g1OptionList);
        gsApproxG1BasisEdge<real_t> g1BasisEdge_1(test_mp.patch(1), test_mb.basis(1), 0, false, g1OptionList);
        gsMultiPatch<> g1Basis_0, g1Basis_1;

        g1BasisEdge_0.setG1BasisEdge(g1Basis_0);
        g1BasisEdge_1.setG1BasisEdge(g1Basis_1);

        if (g1OptionList.getInt("gluingData")==gluingData::global)
            gluingDataCondition(g1BasisEdge_0.get_alpha(),g1BasisEdge_1.get_alpha(),g1BasisEdge_0.get_beta(),g1BasisEdge_1.get_beta());

        //g1BasisEdge_0.plotGluingData(0);

//      Patch 0 -> Right
        auxGeom[0].parametrizeBasisBack(g1Basis_0);

//      Patch 1 -> Left
        auxGeom[1].parametrizeBasisBack(g1Basis_1);

        if (g1OptionList.getInt("gluingData")==gluingData::global)
            g1ConditionRep(g1BasisEdge_0.get_alpha(),g1BasisEdge_1.get_alpha(),g1Basis_0,g1Basis_1);

    }


    void computeG1BoundaryBasis(gsG1OptionList g1OptionList, const int boundaryInd){
        gsMultiPatch<> test_mp(this->reparametrizeG1Boundary(boundaryInd));
        gsMultiBasis<> test_mb(test_mp);

        gsApproxG1BasisEdge<real_t> g1BasisEdge(test_mp, test_mb, 1, true, g1OptionList);
        gsMultiPatch<> g1Basis_edge;
        g1BasisEdge.setG1BasisEdge(g1Basis_edge);

        auxGeom[0].parametrizeBasisBack(g1Basis_edge);
    }

    gsG1AuxiliaryPatch & getSinglePatch(const unsigned i){
        return auxGeom[i];
    }

    void gluingDataCondition(gsBSpline<> alpha_0, gsBSpline<> alpha_1, gsBSpline<> beta_0, gsBSpline<> beta_1)
    {
        // BETA
        // first,last,interior,mult_ends,mult_interior,degree
        gsBSplineBasis<> basis_edge = dynamic_cast<gsBSplineBasis<> &>(auxGeom[0].getPatch().basis().component(1)); // 0 -> v, 1 -> u
        index_t m_p = basis_edge.maxDegree(); // Minimum degree at the interface // TODO if interface basis are not the same

        gsKnotVector<> kv(0, 1, basis_edge.numElements()-1, 2 * m_p  + 1, 2 * m_p - 1 );
        gsBSplineBasis<> bsp(kv);

        gsMatrix<> greville = bsp.anchors();
        gsMatrix<> uv1, uv0, ev1, ev0;

        const index_t d = 2;
        gsMatrix<> D0(d,d);

        gsGeometry<>::Ptr beta_temp;

        uv0.setZero(2,greville.cols());
        uv0.bottomRows(1) = greville;

        uv1.setZero(2,greville.cols());
        uv1.topRows(1) = greville;

        const gsGeometry<> & P0 = auxGeom[0].getPatch(); // iFace.first().patch = 1
        const gsGeometry<> & P1 = auxGeom[1].getPatch(); // iFace.second().patch = 0
        // ======================================

        // ======== Determine bar{beta} ========
        for(index_t i = 0; i < uv1.cols(); i++)
        {
            P0.jacobian_into(uv0.col(i),ev0);
            P1.jacobian_into(uv1.col(i),ev1);

            D0.col(1) = ev0.col(0); // (DuFL, *)
            D0.col(0) = ev1.col(1); // (*,DuFR)

            uv0(0,i) = D0.determinant();
        }

        beta_temp = bsp.interpolateData(uv0.topRows(1), uv0.bottomRows(1));
        gsBSpline<> beta = dynamic_cast<gsBSpline<> &> (*beta_temp);


        index_t p_size = 10000;
        gsMatrix<> points(1, p_size);
        points.setRandom();
        points = points.array().abs();

        gsVector<> vec;
        vec.setLinSpaced(p_size,0,1);
        points = vec.transpose();

        gsMatrix<> temp;
        temp = alpha_1.eval(points).cwiseProduct(beta_0.eval(points))
            + alpha_0.eval(points).cwiseProduct(beta_1.eval(points))
            - beta.eval(points);


        gsInfo << "Conditiontest Gluing data: \n" << temp.array().abs().maxCoeff() << "\n\n";


    }

    void g1ConditionRep(gsBSpline<> alpha_0, gsBSpline<> alpha_1, gsMultiPatch<> g1Basis_0,  gsMultiPatch<> g1Basis_1)
    {
        // BETA
        // first,last,interior,mult_ends,mult_interior,degree
        gsBSplineBasis<> basis_edge = dynamic_cast<gsBSplineBasis<> &>(auxGeom[0].getPatch().basis().component(1)); // 0 -> v, 1 -> u
        index_t m_p = basis_edge.maxDegree(); // Minimum degree at the interface // TODO if interface basis are not the same

        gsKnotVector<> kv(0, 1, basis_edge.numElements()-1, 2 * m_p  + 1, 2 * m_p - 1 );
        gsBSplineBasis<> bsp(kv);

        gsMatrix<> greville = bsp.anchors();
        gsMatrix<> uv1, uv0, ev1, ev0;

        const index_t d = 2;
        gsMatrix<> D0(d,d);

        gsGeometry<>::Ptr beta_temp;

        uv0.setZero(2,greville.cols());
        uv0.bottomRows(1) = greville;

        uv1.setZero(2,greville.cols());
        uv1.topRows(1) = greville;

        const gsGeometry<> & P0 = auxGeom[0].getPatch(); // iFace.first().patch = 1
        const gsGeometry<> & P1 = auxGeom[1].getPatch(); // iFace.second().patch = 0
        // ======================================

        // ======== Determine bar{beta} ========
        for(index_t i = 0; i < uv1.cols(); i++)
        {
            P0.jacobian_into(uv0.col(i),ev0);
            P1.jacobian_into(uv1.col(i),ev1);

            D0.col(1) = ev0.col(0); // (DuFL, *)
            D0.col(0) = ev1.col(1); // (*,DuFR)

            uv0(0,i) = D0.determinant();
        }

        beta_temp = bsp.interpolateData(uv0.topRows(1), uv0.bottomRows(1));
        gsBSpline<> beta = dynamic_cast<gsBSpline<> &> (*beta_temp);



        index_t p_size = 10000;
        gsMatrix<> points(1, p_size);
        points.setRandom();
        points = points.array().abs();

        gsVector<> vec;
        vec.setLinSpaced(p_size,0,1);
        points = vec.transpose();

        gsMatrix<> points2d_0(2, p_size);
        gsMatrix<> points2d_1(2, p_size);

        points2d_0.setZero();
        points2d_1.setZero();
        points2d_0.row(1) = points; // v
        points2d_1.row(0) = points; // u

        real_t g1Error = 0;

        for (size_t i = 0; i < g1Basis_0.nPatches(); i++)
        {
            gsMatrix<> temp;
            temp = alpha_1.eval(points).cwiseProduct(g1Basis_0.patch(i).deriv(points2d_0).topRows(1))
                + alpha_0.eval(points).cwiseProduct(g1Basis_1.patch(i).deriv(points2d_1).bottomRows(1))
                + beta.eval(points).cwiseProduct(g1Basis_0.patch(i).deriv(points2d_0).bottomRows(1));

            if (temp.array().abs().maxCoeff() > g1Error)
                g1Error = temp.array().abs().maxCoeff();
        }

        gsInfo << "Conditiontest G1 continuity Rep: \n" << g1Error << "\n\n";
    }


protected:
    std::vector<gsG1AuxiliaryPatch> auxGeom;
};
}

