MODULE MainModule

VAR robtarget home_pose;
VAR robtarget new_target;


PROC main()

		VAR num current_cmd:=0;
		MoveJ home_pose, v500, z30, tool0;
		init_server;
		wait_for_client \wait_time:=5;

		WHILE (TRUE) DO
		    current_cmd:= wait_for_command();
		    TEST current_cmd
		    CASE MOVE_J:
			     new_target := recv_target();
		         MoveJ new_target, v500, z30, tool0;
		         send_result(0)
		    CASE MOVE_L:
		         new_target := recv_target();
		         MoveL new_target, v500, z30, tool0;
		         send_result(0)
		    CASE CURRENT_POSE:
		        send_position(CRobT(\TaskName:=robot_task_id \Tool:=tool0 \WObj:=wobj0))
		    DEFAULT:
		        TPWrite "Undefined command...";
		        ExitCycle;
		    ENDTEST
		ENDWHILE
ENDPROC

ENDMODULE
