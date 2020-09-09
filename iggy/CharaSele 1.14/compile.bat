mxmlc -compiler.source-path=.\scripts -omit-trace-statements=false .\scripts\CHARASELE_fla\MainTimeline.as
move .\scripts\CHARASELE_fla\MainTimeline.swf .\CHARASELE.swf
iggy_as3_test CHARASELE.swf
copy /Y CHARASELE.iggy "C:\Program Files (x86)\Steam\steamapps\common\DB Xenoverse 2\data\ui\iggy\CHARASELE.iggy"
