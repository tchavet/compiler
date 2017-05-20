; ModuleID = 'main.vsop'

@true.str = constant [6 x i8] c"true\0a\00"
@false.str = constant [7 x i8] c"false\0a\00"
@.iostr = private unnamed_addr constant [3 x i8] c"%d\00"
@.iostr2 = private unnamed_addr constant [3 x i8] c"%s\00"

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
	call i32 @puts(i8* %str)
	ret %class.IO* %io.ptr.1
}

define fastcc %class.IO* @method.IO.printInt32(%class.IO* %io.ptr.2, i32 %nb)
{
	ret %class.IO* %io.ptr.2
}

define fastcc %class.IO* @method.IO.printBool(%class.IO* %io.ptr.3, i1 %b)
{
	br i1 %b, label %if_true, label %if_false
	if_true:
		%true.ptr = getelementptr [6 x i8]* @true.str, i32 0, i32 0
		br label %if_end
	if_false:
		%false.ptr = getelementptr [7 x i8]* @false.str, i32 0, i32 0
		br label %if_end
	if_end:
		%toPrint = phi i8* [%true.ptr, %if_true], [%false.ptr, %if_false]
	%io.ptr.4 = call fastcc %class.IO* @method.IO.print(%class.IO* %io.ptr.3, i8* %toPrint)
	ret %class.IO* %io.ptr.4
}

define fastcc i32 @method.IO.inputInt32(%class.IO* %io.ptr.5)
{	
    %io.3 = alloca i32, align 4
	%io.4 = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.iostr, i32 0, i32 0), i32* %io.3)
	ret i32 %io.4
}

define fastcc i1 @method.IO.inputBool(%class.IO* %io.ptr.5)
{
  %io.5 = alloca i8*, align 8
  %io.6 = alloca i8, align 1
  store i32 0, i32* %io.5
  store i8 1, i8* %io.5, align 1
  %io.7 = load i8** %io.5, align 8
  %io.8 = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.iostr2, i32 0, i32 0), i8* %io.7)
  %io.8 = load i8** %io.5, align 8
  %io.9 = call i32 @strcmp(i8* %io.8, i8* getelementptr inbounds ([5 x i8]* @true.str, i32 0, i32 0)) #3
  %io.10 = icmp ne i32 %io.9, 0
  br i1 %io.10, label %io.11, label %io.15

; <label>:io.11                                       ; preds = %0
  %io.12 = load i8* %t, align 1
  %io.13 = trunc i8 %io.12 to i1
  %io.14 = zext i1 %io.13 to i32
  store i32 %io.14, i32* %io.5
  br label %io.19

; <label>:io.14                                      ; preds = %0
  %io.15 = load i8* %io.6, align 1
  %io.16 = trunc i8 %io.15 to i1
  %io.17 = xor i1 %io.16, true
  %io.18 = zext i1 %io.17 to i32
  store i32 %io.18, i32* %1
  br label %io.19

; <label>:16                                      ; preds = %io.15, %io.11
  %io.20 = load i32* %1
  ret i1 %io.20
}

define fastcc i8* @method.IO.inputLine(%class.IO* %io.ptr.6)
{
	%io.5 = alloca i8*, align 8
	%io.6 = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.iostr2, i32 0, i32 0), i8* %io.5)
	ret i8* %io.6
}
