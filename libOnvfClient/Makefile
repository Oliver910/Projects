

CXXFLAGS=-Wno-format-security  -I./auth -I./local/include/libxml2

OBJS=Test.o COnVifClient.o \
	COnVifDeviceMngmt.o \
	COnVifEvents.o \
	COnVifImagingConf.o \
	COnVifMediaConf.o \
	COnVifPTZ.o \
	CUrlTool.o \
	fOnVifToolBase.o \
	fSocketTool.o \
	CSocketIPv4.o \
	CHtBase64.o \
	CDigestInfo.o \
	auth/digcalc.o  \
	auth/HMAC_SHA1.o  \
	auth/md5.o  \
	auth/SHA1.o

LIBS=local/lib/libxml2.a


TARGET=test

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LD_FLAGS) $(LIBS)

clean:
	rm  -f $(OBJS) $(TARGET) *.o

