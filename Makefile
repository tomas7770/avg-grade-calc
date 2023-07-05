STATIC = 0
STATIC_FLAG = 
WX_CONFIG = wx-config
OUT_NAME = bin/AvgCalc
RC_OBJECT = 
ifeq ($(STATIC),1)
	STATIC_FLAG = -static
	WX_CONFIG = wx-config-static
endif
ifeq ($(OS),Windows_NT)
	OUT_NAME = bin/AvgCalc.exe
	RC_OBJECT = bin/AvgCalc_rc.o
endif

calc:
	mkdir -p bin
ifeq ($(OS),Windows_NT)
	windres --include-dir "/ucrt64/include/wx-3.0" src/AvgCalc.rc -o $(RC_OBJECT)
endif
	g++ src/*.cpp `$(WX_CONFIG) --cxxflags --libs` $(STATIC_FLAG) -o $(OUT_NAME) $(RC_OBJECT)
clean:
	rm bin/*
