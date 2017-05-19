; ModuleID = 'main.vsop'

@true.str = constant [6 x i8] c"true\0a\00"
@false.str = constant [7 x i8] c"false\0a\00"

define fastcc %class.IO* @method.IO.print(%class.IO*, i8* %str)
{
	call i32 @puts(i8* %str)
	%io = alloca %class.IO, align 8
	%ioPtr = getelementptr %class.IO* %io, i32 0, i32 0
	store %methods.type.IO* %methods.IO, %methods.type.IO** %ioPtr
	ret %class.IO* %io
}

define fastcc %class.IO* @method.IO.printInt32(%class.IO*, i32 %nb)
{
	call i32 @puts(i32 %nb)
	%io.1 = alloca %class.IO
	%ioPtr.1 = getelementptr %class.IO* %io.1, i32 0, i32 0
	store %methods.type.IO* %methods.IO, %methods.type.IO** %ioPtr.1
	ret %class.IO* %io.1
}

define fastcc %class.IO* @method.IO.printBool(%class.IO*, i1 %b)
{
	br i1 %b, label if_true, label if_false
	if_true:
		%true.ptr = getelementptr [6 x i8]* %true.str, i32 0, i32 0
		br label if_end
	if_false:
		%false.ptr = getelementptr [7 x i8]* %false.str, i32 0, i32 0
		br label if_end
	if_end:
		%toPrint = phi i8* [%true.ptr, if_true], [%false.ptr, if_false]
	%io.2 = call fastcc %class.IO* @method.IO.print(i8* %toPrint)
	ret %class.IO* %io.2
}

define fastcc i32 @method.IO.inputInt32(%class.IO*)
{
	ret 0
}

define fastcc i1 @method.IO.inputBool(%class.IO*)
{
	ret 0
}

define fastcc i8* @method.IO.inputLine(%class.IO*)
{
	ret 0
}
