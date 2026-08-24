const Standard=@import("std");
const Chameleon=@import("Chameleon");

pub var WhetherColour=false;
pub var Console:Chameleon.RuntimeChameleon=undefined;

pub fn Initialize(GPA:Standard.mem.Allocator,IO:Standard.Io)void{
	Console=Chameleon.initRuntime(.{.allocator=GPA,.io=IO,.no_color=!WhetherColour});
}

pub fn Red(comptime TheString:[]const u8,Arguments:anytype)void{
	if(WhetherColour){
		Console.red().bold().printOut(TheString,Arguments) catch {};
	}
	else{
		Console.printOut("Red: ",.{}) catch {};
		Console.printOut(TheString,Arguments) catch {};
	}
}