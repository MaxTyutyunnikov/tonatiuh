/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef TANALYZERKIT_H_
#define TANALYZERKIT_H_

#include <Inventor/fields/SoSFVec3s.h>

#include "trt.h"
#include "TSeparatorKit.h"

class QMutex;
class RandomDeviate;
class Ray;
class SceneModel;
class SceneModel;
class Transform;

//using namespace std;

//!  TAnalyzerKit class groups what is necessary to the shape.
/*!
  TAnalyzerKit groups the shape geometry, material and the transformation.

*/

class TAnalyzerKit : public TSeparatorKit
{
	typedef TSeparatorKit inherited;

    SO_KIT_HEADER(TAnalyzerKit);
 	SO_KIT_CATALOG_ENTRY_HEADER( parameter );
 	SO_KIT_CATALOG_ENTRY_HEADER( result );
 	SO_KIT_CATALOG_ENTRY_HEADER( levelList );

public:
    TAnalyzerKit();
    static void initClass();

    //void Compute( vector< vector<Ray> >* raysWays, Transform* m_transformWTO, QMutex* mutex );
    void DisplayResults();
    QString GetIcon() const;
    SoNodeKitListPart* GetLevelList();
    TSeparatorKit* GetResultsList();
    bool IntersectP( const Ray& ray ) const;
    void FinalizeCompute( double raydensity, Transform* m_transformWTO );
    void PrepareCompute( SceneModel * pModel );
	void RemoveResult( SceneModel* pModel );
    void ResetValues();
	void UpdateSize( SceneModel* pModel );


private:
    virtual ~TAnalyzerKit();
    SoSFVec3s previousSplit;
	trt::TONATIUH_REAL previousElemSize;

};

#endif /*TANALYZERKIT_H_*/
