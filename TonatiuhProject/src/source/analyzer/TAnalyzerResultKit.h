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

#ifndef TANALYSERRESULTKIT_H_
#define TANALYSERRESULTKIT_H_

#include <vector>

#include <QMutex>

#include "TShapeKit.h"
#include "trt.h"
#include "Transform.h"
#include "TAnalyzerResult.h"
#include "TAnalyzerParameter.h"
#include "TShape.h"


class RandomDeviate;
class Ray;

//!  TAnalyzerResultKit class groups what is necessary to the shape.
/*!
  TAnalyzerResultKit groups the shape geometry, material and the transformation.

*/

class TAnalyzerResultKit : public TShapeKit
{
	typedef TShapeKit inherited;

    SO_KIT_HEADER(TAnalyzerResultKit);
 	SO_KIT_CATALOG_ENTRY_HEADER( result );
    //SO_KIT_CATALOG_ENTRY_HEADER( splitList );


public:
    TAnalyzerResultKit();
    static void initClass();

    bool IntersectP( const Ray& ray ) const;
    void Compute( std::vector<Ray> *raysWays, Transform *m_transformWTO, QMutex* mutex);
    void PrepareCompute();
    void FinalizeCompute(double raydensity,  Transform *m_transformWTO);
    void DisplayResult(int typeOfDisplay, SoNodeKitListPart *levelList);
    void ResetValues();
    QString getIcon();


private:
    virtual ~TAnalyzerResultKit();

    TAnalyzerResult * analyzerResult;
    TShape* tshape;

};

#endif /*TANALYSERRESULTKIT_H_*/
