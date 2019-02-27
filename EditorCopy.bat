cd..

xcopy .\EngineWrapper\Bin\*.* .\WinFormEditor\bin\Debug\ /s /y /d /exclude:exclude.txt
xcopy .\EngineWrapper\Bin\*.* .\WinFormEditor\bin\Release\ /s /y /d /exclude:exclude.txt