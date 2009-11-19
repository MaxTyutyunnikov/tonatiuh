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

#ifndef TESTVECTOR3D_H_
#define TESTVECTOR3D_H_

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class Vector3D;

class TestVector3D : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TestVector3D);
		CPPUNIT_TEST(testEquality);
		CPPUNIT_TEST(testInequality);
		CPPUNIT_TEST(testAddVector);
	   	CPPUNIT_TEST(testDotProduct);
		CPPUNIT_TEST(testDotProductWithNormal);
		CPPUNIT_TEST(testAbsDotProduct);
		CPPUNIT_TEST(testLengthSquared);
		CPPUNIT_TEST(testLength);
		CPPUNIT_TEST(testCrossProduct);
		CPPUNIT_TEST(testCrossProductWithNormal);
		CPPUNIT_TEST(testNormalize);
		CPPUNIT_TEST(testSameHemisphere);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

protected:
	void testEquality();
	void testInequality();
	void testAddVector();
	void testDotProduct();
	void testDotProductWithNormal();
	void testAbsDotProduct();
	void testCrossProduct();
	void testCrossProductWithNormal();
	void testLengthSquared();
	void testLength();
	void testNormalize();
	void testSameHemisphere();

private:
	Vector3D* m_p1;
	Vector3D* m_p2;
	Vector3D* m_p3;
};

#endif /*TESTVECTOR3D_H_*/
