TEMPLATE = subdirs


SUBDIRS =  ComponentHeliostatField \
			MaterialBasicRefractive \
			MaterialStandardSpecular \
            MaterialStandardRoughSpecular \
            MaterialVirtual \
			PhotonMapExportDB \
			PhotonMapExportFile \
			PhotonMapExportNull\
			RandomMersenneTwister \
			RandomRngStream \
            ShapeBezierSurface \
			ShapeCone \
			ShapeCylinder \
			ShapeFlatDisk \
			ShapeFlatRectangle \
            ShapeFlatTriangle \
			ShapeHyperboloid \
			ShapeParabolicDish \
			ShapeParabolicRectangle \
			ShapeSphere \
			ShapeSphericalPolygon \
            ShapeSphericalRectangle \
            ShapeTroughAsymmetricCPC \
			ShapeTroughCHC \
            ShapeTroughCPC \
            ShapeTroughHyperbola \
            ShapeTroughParabola \
            ShapeTrumpet \
            SunshapeBuie \
			SunshapePillbox \
			TrackerHeliostat \
            TrackerLinearFresnel \
			TrackerOneAxis \
            TransmissivityATMParameters \
            TransmissivityBallestrin \
            TransmissivityDefault \
            TransmissivityMirval \
            TransmissivitySenguptaNREL \
            TransmissivityVantHull \
            TransmissivityVittitoeBiggs

plugins.target= plugins
plugins.depends = src

QMAKE_EXTRA_TARGETS += plugins