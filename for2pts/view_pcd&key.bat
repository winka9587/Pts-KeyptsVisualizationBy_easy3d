@echo off
setlocal enabledelayedexpansion

set "exe_path=E:\JL\38\exe\two_pcd_viewer.exe"
set "search_path=E:\JL\38"
set "save_path=E:\JL\result3\"
for /r "%search_path%" %%a in (*.ply) do (
    set "file_path=%%~dpnxa"
    set "parent_path=%%~dpa"
    set "name=%%~na"

    %exe_path% !parent_path! !save_path!
)

endlocal
pause
