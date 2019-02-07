cd..

xcopy .\Mathlib\Include\*.h .\Engine\Include\ /s /y /d
xcopy .\Mathlib\Include\*.h .\Math\Include\ /s /y /d /exclude:exclude.txt
xcopy .\Mathlib\Bin\*.* .\Math\bin\ /s /y /d /exclude:exclude.txt
xcopy .\Mathlib\Bin\*.* .\Engine\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Mathlib\Bin\*.* .\TWEditor\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Mathlib\Bin\*.* .\3DClient\bin\ /s /y /d /exclude:exclude.txt