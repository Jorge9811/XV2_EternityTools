CC = gcc
CXX = g++

OUTPUT := genser.exe
OBJS = main.o debug.o ../eternity_common/Utils.o ../eternity_common/BaseFile.o 
OBJS += ../eternity_common/crypto/sha1.o ../eternity_common/crypto/md5.o ../eternity_common/crypto/rijndael.o
OBJS += ../eternity_common/Stream.o ../eternity_common/MemoryStream.o ../eternity_common/FixedMemoryStream.o ../eternity_common/FileStream.o ../eternity_common/BitStream.o ../eternity_common/FixedBitStream.o
OBJS += ../eternity_common/Criware/CrilaylaFixedBitStream.o ../eternity_common/Criware/CpkFile.o ../eternity_common/Criware/UtfFile.o
OBJS += ../eternity_common/SSSS/2ryFile.o ../eternity_common/SSSS/BgrFile.o ../eternity_common/DBXV2/CmsFile.o ../eternity_common/SSSS/CscFile.o ../eternity_common/SSSS/CspFile.o ../eternity_common/DBXV2/CusFile.o
OBJS += ../eternity_common/DBXV2/CsoFile.o ../eternity_common/DBXV2/BcsFile.o ../eternity_common/DBXV2/PscFile.o ../eternity_common/DBXV2/HciFile.o ../eternity_common/DBXV2/SevFile.o ../eternity_common/DBXV2/CmlFile.o
OBJS += ../eternity_common/Dimps/EmaFile.o ../eternity_common/Dimps/EmbFile.o ../eternity_common/Dimps/EmgFile.o ../eternity_common/Dimps/EmmFile.o ../eternity_common/Dimps/EmoFile.o ../eternity_common/DBXV2/CncFile.o 
OBJS += ../eternity_common/DBXV2/CnsFile.o ../eternity_common/DBXV2/ErsFile.o
OBJS += ../eternity_common/DBXV2/BaiFile.o ../eternity_common/DBXV2/TnlFile.o ../eternity_common/DBXV2/TsrFile.o ../eternity_common/DBXV2/TsdFile.o ../eternity_common/DBXV2/TtbFile.o ../eternity_common/DBXV2/TtcFile.o
OBJS += ../eternity_common/DBXV2/MsgFile.o ../eternity_common/Dimps/SkeletonFile.o ../eternity_common/SSSS/SpmFile.o ../eternity_common/SSSS/TdbFile.o ../eternity_common/SSSS/VpmFile.o ../eternity_common/SSSS/VstFile.o
OBJS += ../eternity_common/IniFile.o ../eternity_common/DBXV2/AurFile.o ../eternity_common/DBXV2/CharaListFile.o ../eternity_common/Misc/IggyFile.o ../eternity_common/SwfFile.o
OBJS += ../eternity_common/DBXV2/Xv2PreBakedFile.o ../eternity_common/DBXV2/IdbFile.o ../eternity_common/DBXV2/PalFile.o ../eternity_common/DBXV2/BacFile.o ../eternity_common/DBXV2/PupFile.o ../eternity_common/DBXV2/PsaFile.o
OBJS += ../eternity_common/DBXV2/AitFile.o ../eternity_common/DBXV2/BcmFile.o ../eternity_common/DBXV2/X2mCostumeFile.o ../eternity_common/DBXV2/Xv2PatcherSlotsFile.o 
OBJS += ../eternity_common/DBXV2/Xv2StageDefFile.o ../eternity_common/DBXV2/Xv2PatcherSlotsFileStage.o
OBJS += ../eternity_common/Criware/AcbFile.o ../eternity_common/Criware/Afs2File.o ../eternity_common/Criware/HcaFile.o ../eternity_common/WavFile.o
OBJS += ../eternity_common/Thread.o
OBJS += ../eternity_common/SSSS/SsssData.o ../eternity_common/DBXV2/Xenoverse2.o ../eternity_common/Criware/CriFs.o ../eternity_common/DBXV2/Xv2Fs.o
OBJS += ../eternity_common/tinyxml/tinyxml.o ../eternity_common/tinyxml/tinystr.o ../eternity_common/tinyxml/tinyxmlerror.o ../eternity_common/tinyxml/tinyxmlparser.o
CFLAGS=-Wall -I. -I../eternity_common -std=gnu99 -mno-ms-bitfields -O2 -masm=intel 
CFLAGS += -static
CXXFLAGS=-Wall -Wno-strict-aliasing -I../eternity_common -O2 -mno-ms-bitfields -std=c++11 -DTIXML_USE_STL -DNO_ZLIB -DXV2 -DSSSS
LDFLAGS=-L. 
LIBS =  -lstdc++ -lversion

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) *.o
	rm -f ../eternity_common/*.o
	rm -f ../eternity_common/Misc/*.o
	rm -f ../eternity_common/DBXV2/*.o
	rm -f ../eternity_common/Criware/*.o
	rm -f ../eternity_common/Dimps/*.o
	rm -f ../eternity_common/tinyxml/*.o	
	rm -f ../eternity_common/crypto/*.o

$(OUTPUT): $(OBJS)
	$(LINK.c) $(LDFLAGS) -o $@ $^ $(LIBS)
