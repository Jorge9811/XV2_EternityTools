mxmlc -compiler.source-path=.\scripts -omit-trace-statements=false .\scripts\POPUP_PAUSE_fla\MainTimeline.as
move .\scripts\POPUP_PAUSE_fla\MainTimeline.swf .\POPUP_PAUSE.swf
iggy_as3_test POPUP_PAUSE.swf
copy /Y POPUP_PAUSE.iggy "C:\Program Files (x86)\Steam\steamapps\common\DB Xenoverse 2\data\ui\iggy\POPUP_PAUSE.iggy"
