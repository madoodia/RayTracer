# =================================== #
# (C) 2019 madoodia@gmail.com.
# All rights reserved.
# =================================== #


# ------------------------------------------------
PROJECT_NAME       = $(FOLDER_NAME)

OBJECTS_SRC        = src/main.obj

OBJECTS_OUT        = main.obj

USELESS_EXT        = *.obj \
                     *.lib \
                     *.exp \
                     *.idb \
                     *.pdb \
                     *.ilk \
                     *.cpp \
                     *.h

# ------------------------------------------------
EXTRA_WIN_LIBS	    = opengl32.lib
DEFAULT_WIN_LIBS	= kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

# ------------------------------------------------
TARGET             = $(PROJECT_NAME).exe
INCLUDES           = -I.
LIBS               = $(EXTRA_WIN_LIBS) $(DEFAULT_WIN_LIBS)
LINKER_FLAGS       = -nologo  -MACHINE:X64
DEL                 = del
LINKER              = link

# ------------------------------------------------
CC           = cl
DEFINES      = -DUNICODE -DWIN32 -D_WIN64 -D_WINDOWS -DNT_PLUGIN
CC_FLAGS     = -Zc:wchar_t -O2 -FC -MT -W3 -GR -EHs $(DEFINES) # Warning Level 3
# CC_FLAGS     = -Zc:wchar_t -O2 -FC -MD -Wall -GR -EHs $(DEFINES) # Full warningg


.SUFFIXES: .cpp .c .obj .exe

.cpp.obj:
    $(CC) -c $(INCLUDES) $(CC_FLAGS) @<<
    $<
<<

all: Makefile run cleanup

run:  $(OBJECTS_SRC)
    $(LINKER) $(LINKER_FLAGS) -OUT:$(TARGET) $(OBJECTS_OUT) $(LIBS)

cleanup:
    -$(DEL) $(USELESS_EXT)