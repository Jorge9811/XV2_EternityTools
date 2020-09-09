mxmlc -compiler.source-path=.\scripts -omit-trace-statements=false .\scripts\STAGESELE_fla\MainTimeline.as
move .\scripts\STAGESELE_fla\MainTimeline.swf .\STAGESELE.swf
iggy_as3_test STAGESELE.swf
copy /Y STAGESELE.iggy "C:\Program Files (x86)\Steam\steamapps\common\DB Xenoverse 2\data\ui\iggy\STAGESELE.iggy"
