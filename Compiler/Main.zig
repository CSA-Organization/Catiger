const Standard=@import("std");
const Output=@import("Output.zig");

pub fn main(Initialization:Standard.process.Init)u8{
	const GPA=Initialization.gpa;
	const IO=Initialization.io;

	const NoColour=Initialization.minimal.environ.getPosix("NO_COLOR");
	if((Initialization.minimal.environ.getPosix("CLICOLOR_FORCE")!=null)or(NoColour==null or NoColour.?.len==0)){
		Output.WhetherColour=true;
	}
	else{
		Output.WhetherColour=false;
	}

	Output.Initialize(GPA,IO);

	if(Initialization.minimal.args.vector.len==1){
		Output.Red("Arguments is only 1 and not enough, try help",.{});
		return 1;
	}
	else{
		return 0;
	}

	return 0;
}