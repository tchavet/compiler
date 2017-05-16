; ModuleID = 'vsopl'

define fastcc @class.IO* @method.IO.print(i8* %str)
{
	call i32(i8*)* @puts(i8* %str)
	%io = alloca @class.IO
	%ioPtr = getelementptr @class.IO* %io, i32 0, i32 0
	store @methods.type.IO @methods.IO, @methods.type.IO* %ioPtr
	ret %io
}

define fastcc @class.IO* @method.IO.printInt32(i32 %nb)
{
	call i32(i32)* @puts(i32 %nb)
	%io.1 = alloca @class.IO
	%ioPtr.1 = getelementptr @class.IO* %io.1, i32 0, i32 0
	store @methods.type.IO @methods.IO, @methods.type.IO* %ioPtr.1
	ret %io.1
}

define fastcc @class.IO* @method.IO.printBool(i1 %b)
{
	br i1 %b, label if_true, label if_false
	if_true:
		%true.str = constant [8 x i8] c"true\x0a\00"
		%true.ptr = getelementptr [8 x i8]* %true.str, i32 0, i32 0
		br label if_end
	if_false:
		%false.str = constant [9 x i8] c"false\x0a\00"
		%false.ptr = getelementptr [9 x i8]* %false.str, i32 0, i32 0
		br label if_end
	if_end:
		%toPrint = phi i8* [%true.ptr, if_true], [%false.ptr, if_false]
	%io.2 = call fastcc @class.IO* (i8*)* @method.IO.print(i8* %toPrint)
	ret %io.2
}

define fastcc i32 @method.IO.inputInt32()
{
	ret 0
}

define fastcc i1 @method.IO.inputBool()
{
	ret 0
}

define fastcc i8* @method.IO.inputLine()
{
	ret 0
}
