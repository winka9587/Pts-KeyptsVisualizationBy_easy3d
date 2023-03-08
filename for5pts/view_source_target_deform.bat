@echo off
setlocal enabledelayedexpansion

set "exe_path=E:\JL\keypoints_viewer\viewer.exe"
set "search_path=E:\JL\keypoints"
set "save_path=E:\JL\result2\"
for /r "%search_path%" %%a in (*.ply) do (
    set "file_path=%%~dpnxa"
    set "parent_path=%%~dpa"
    set "name=%%~na"

    %exe_path% !parent_path! !save_path!
)

endlocal
pause
