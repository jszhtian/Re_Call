@echo off

@rem ============================================================
@rem                     Configuration File
@rem ============================================================

@rem ============================================================
@rem                     Environment variables & const define
@rem ============================================================
set WORK_SPACE=F:\Round Aound the World\build


@rem ============================================================
@rem                     Path define
@rem ============================================================
set PC_RESOURCE=F:\Round Aound the World
set DIR_BUILD=%WORK_SPACE%\build
set DIR_OUTPUT=%WORK_SPACE%\output
set DIR_PYTHON=D:\Python27
set DIR_PERL=D:\Perl\bin
set DIR_TEMP=%WORK_SPACE%\temp

path %PATH%;%DIR_PYTHON%;%DIR_PERL%
@echo on