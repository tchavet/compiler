; ModuleID = 'main.vsop'

@true.str = constant [5 x i8] c"true\00"
@false.str = constant [6 x i8] c"false\00"
@.iostr = private unnamed_addr constant [3 x i8] c"%d\00"
@.iostr2 = private unnamed_addr constant [3 x i8] c"%s\00"
@.iostr3 = private unnamed_addr constant [4 x i8] c"%s\0A\00"
;@.strstr = private unnamed_addr constant [2 x i8] c"%s"
@.intstr = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.intstr2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.printstr = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@.boolstr = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@stdin = external global %struct._IO_FILE*
declare i32 @__isoc99_scanf(i8*, ...) #1
; Function Attrs: nounwind readonly
declare i32 @strcmp(i8*, i8*) #2
declare i32 @puts(i8*) #3
declare i32 @printf(i8*, ...)
declare i32 @getline(i8**, i32*, %struct._IO_FILE*)

%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i32, i32, [40 x i8] }
%method.type.IO.print = type %class.IO* (%class.IO*, i8*)*
%method.type.IO.printInt32 = type %class.IO* (%class.IO*, i32)*
%method.type.IO.printBool = type %class.IO* (%class.IO*, i1)*
%method.type.IO.inputInt32 = type i32 (%class.IO*)*
%method.type.IO.inputBool = type i1 (%class.IO*)*
%method.type.IO.inputLine = type i8* (%class.IO*)*
%methods.type.IO = type {%method.type.IO.print, %method.type.IO.printInt32, %method.type.IO.printBool, %method.type.IO.inputInt32, %method.type.IO.inputBool, %method.type.IO.inputLine}
@methods.IO = external global %methods.type.IO
%class.IO = type {%methods.type.IO*}

define fastcc %class.IO* @method.IO.print(%class.IO* %io.ptr.1, i8* %str)
{
	call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.printstr, i32 0, i32 0), i8* %str)
	ret %class.IO* %io.ptr.1

	;%1 = alloca i8*, align 4
	;store i8* %str, i8** %1, align 4
	;%2 = load i8** %1, align 4
	;%3 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.printstr, i32 0, i32 0), i8* %2)
	;ret %class.IO* %io.ptr.1
}

define fastcc %class.IO* @method.IO.printInt32(%class.IO* %io.ptr.2, i32 %nb)
{
	call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.intstr, i32 0, i32 0), i32 %nb)
	ret %class.IO* %io.ptr.2
}

define fastcc %class.IO* @method.IO.printBool(%class.IO* %io.ptr.3, i1 %b)
{
	br i1 %b, label %if_true, label %if_false
	if_true:
		%true.ptr = getelementptr [5 x i8]* @true.str, i32 0, i32 0
		br label %if_end
	if_false:
		%false.ptr = getelementptr [6 x i8]* @false.str, i32 0, i32 0
		br label %if_end
	if_end:
		%toPrint = phi i8* [%true.ptr, %if_true], [%false.ptr, %if_false]
	%io.ptr.4 = call fastcc %class.IO* @method.IO.print(%class.IO* %io.ptr.3, i8* %toPrint)
	ret %class.IO* %io.ptr.4
}

define fastcc i32 @method.IO.inputInt32(%class.IO* %io.ptr.5)
{	
	;define i32 @readInt() #0 {
  	%ret = alloca i32, align 4
  	%1 = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.intstr, i32 0, i32 0), i32* %ret)
 	%2 = load i32* %ret, align 4
  	ret i32 %2
}

define fastcc i1 @method.IO.inputBool(%class.IO* %io.ptr.5)
{ 
	%1 = alloca i1, align 4
	%bchar = alloca [6 x i8], align 1
	%ctrue = alloca i8*, align 4
	%cfalse = alloca i8*, align 4
	store i8* getelementptr inbounds ([5 x i8]* @true.str, i32 0, i32 0), i8** %ctrue, align 4
	store i8* getelementptr inbounds ([6 x i8]* @false.str, i32 0, i32 0), i8** %cfalse, align 4
	%2 = getelementptr inbounds [6 x i8]* %bchar, i32 0, i32 0
	%3 = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.boolstr, i32 0, i32 0), i8* %2)
	%4 = getelementptr inbounds [6 x i8]* %bchar, i32 0, i32 0
	%5 = load i8** %ctrue, align 4
	%6 = call i32 @strcmp(i8* %4, i8* %5) #3
	%7 = icmp eq i32 %6, 0
	br i1 %7, label %8, label %9

	; <label>:8                                       ; preds = %0
	store i1 1, i1* %1
	br label %16

	; <label>:9                                       ; preds = %0
	%10 = getelementptr inbounds [6 x i8]* %bchar, i32 0, i32 0
	%11 = load i8** %cfalse, align 4
	%12 = call i32 @strcmp(i8* %10, i8* %11) #3
	%13 = icmp eq i32 %12, 0
	br i1 %13, label %14, label %15

	; <label>:14                                      ; preds = %9
	store i1 0, i1* %1
	br label %16

	; <label>:15                                      ; preds = %9
	store i1 0, i1* %1
	br label %16

	; <label>:16                                      ; preds = %15, %14, %8
	%17 = load i1* %1
	ret i1 %17

	;Case for nor true nor false ? 


	;%io.5 = alloca i8, align 8
	;%io.6 = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.iostr2, i32 0, i32 0), i8* %io.5)
;    %io.9 = call i8* @method.IO.inputLine(%class.IO* %io.ptr.5)
	;call %class.IO* @method.IO.print(%class.IO* %io.ptr.5, i8* %io.9)
;    %io.11 = call i32 @strcmp(i8* %io.9, i8* getelementptr inbounds ([5 x i8]* @true.str, i32 0, i32 0)) #3
;    %io.12 = icmp ne i32 %io.11, 0
;    br i1 %io.12, label %io.if_true, label %io.if_false
;    io.if_true:
;    ret i1 1
;    io.if_false:
;    ret i1 0
}

define fastcc i8* @method.IO.inputLine(%class.IO* %io.ptr.6)
{
	;%io.5 = alloca i8, align 8
	;%io.6 = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.iostr2, i32 0, i32 0), i8* %io.5)
	;ret i8* %io.5

	%read = alloca i32, align 4
	%len = alloca i32, align 4
	%line = alloca i8*, align 4
	store i8* null, i8** %line, align 4
	%1 = load %struct._IO_FILE** @stdin, align 4
	%2 = call i32 @getline(i8** %line, i32* %len, %struct._IO_FILE* %1)
	store i32 %2, i32* %read, align 4
	%3 = load i8** %line, align 4
	ret i8* %3
}
