######################################################################
# Automatically generated by qmake (2.01a) mi� 7. feb 13:18:07 2007
######################################################################

TEMPLATE      = lib
CONFIG       += plugin debug_and_release


INCLUDEPATH += . \
				src \
				../Tonatiuh/src \
				$$(TDE_ROOT)/local/include
					
win32{
	DEFINES+= COIN_DLL SOQT_DLL
	

				
	
}
unix{
	INCLUDEPATH += 	. \
					../Tonatiuh/src\ 
					src

	LIBS +=-L/usr/local/lib -lCoin -lSoQt
}

# Input
HEADERS = *.h \
           	../Tonatiuh/src/Trace.h \
           	../Tonatiuh/src/TSunShape.h 


SOURCES = *.cpp \
           	../Tonatiuh/src/Trace.cpp \
           	../Tonatiuh/src/TSunShape.cpp

RESOURCES += SunshapePillbox.qrc	


DESTDIR       = ../Tonatiuh/plugins/SunshapePillbox	
TARGET        = SunshapePillbox

contains(TEMPLATE,lib) {
   CONFIG(debug, debug|release) {
   
		LIBS +=-L$$(TDE_ROOT)/local/lib -lSoQtd -lCoind  
		unix {
			TARGET = $$member(TARGET, 0)_debug
		}
      	else:TARGET = $$member(TARGET, 0)d
	}
	else:LIBS +=-L$$(TDE_ROOT)/local/lib -lSoQt -lCoin  
}
