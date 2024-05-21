	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
_int_tag:
	.word	2
_bool_tag:
	.word	3
_string_tag:
	.word	4
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const11:
	.word	4
	.word	5
	.word	
	.word	int_const2
	.byte	0	
	.align	2
	.word	-1
str_const10:
	.word	4
	.word	10
	.word	
	.word	int_const3
	.ascii	"AAAAAAAAAAAAAAAAAAAA"
	.byte	0	
	.align	2
	.word	-1
str_const9:
	.word	4
	.word	6
	.word	
	.word	int_const4
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const8:
	.word	4
	.word	6
	.word	
	.word	int_const5
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	4
	.word	6
	.word	
	.word	int_const4
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	4
	.word	5
	.word	
	.word	int_const6
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const5:
	.word	4
	.word	5
	.word	
	.word	int_const7
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const4:
	.word	4
	.word	6
	.word	
	.word	int_const5
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const3:
	.word	4
	.word	8
	.word	
	.word	int_const8
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	4
	.word	6
	.word	
	.word	int_const4
	.ascii	"four"
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	4
	.word	8
	.word	
	.word	int_const9
	.ascii	"mylengthisxxxxx"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	4
	.word	7
	.word	
	.word	int_const10
	.ascii	"pa4-try2.cl"
	.byte	0	
	.align	2
	.word	-1
int_const10:
	.word	2
	.word	4
	.word	
	.word	11
	.word	-1
int_const9:
	.word	2
	.word	4
	.word	
	.word	15
	.word	-1
int_const8:
	.word	2
	.word	4
	.word	
	.word	13
	.word	-1
int_const7:
	.word	2
	.word	4
	.word	
	.word	2
	.word	-1
int_const6:
	.word	2
	.word	4
	.word	
	.word	3
	.word	-1
int_const5:
	.word	2
	.word	4
	.word	
	.word	6
	.word	-1
int_const4:
	.word	2
	.word	4
	.word	
	.word	4
	.word	-1
int_const3:
	.word	2
	.word	4
	.word	
	.word	20
	.word	-1
int_const2:
	.word	2
	.word	4
	.word	
	.word	0
	.word	-1
int_const1:
	.word	2
	.word	4
	.word	
	.word	1003434
	.word	-1
int_const0:
	.word	2
	.word	4
	.word	
	.word	1003433
	.word	-1
bool_const0:
	.word	3
	.word	4
	.word	
	.word	0
	.word	-1
bool_const1:
	.word	3
	.word	4
	.word	
	.word	1
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
