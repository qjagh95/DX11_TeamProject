cd..
cd..

xcopy .\TWEngine\Include\*.h .\Engine\Include\ /s /y /d
xcopy .\TWEngine\Bin\*.* .\Engine\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\TWEngine\Bin\*.* .\TWClient\bin\ /s /y /d /exclude:exclude.txt
xcopy .\TWEngine\Bin\*.* .\TWEditor\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\TWEngine\Bin\*.* .\3DClient\bin\ /s /y /d /exclude:exclude.txt