######################################################################
# Automatically generated by qmake (2.01a) mi� 7. feb 13:18:07 2007
######################################################################

TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri)

INCLUDEPATH += . \
			src \
			$$(TONATIUH_ROOT)/src

# Input
HEADERS = src/*.h

SOURCES = src/*.cpp 

TARGET        = RandomRngStream

contains(TEMPLATE,lib) {  

	CONFIG(debug, debug|release) {
		DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/RandomRngStream	

	}
	else { 
		DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/RandomRngStream
	}
}
