const Standard=@import("std");
const Chameleon=@import("Chameleon");

pub fn main(Initialization: Standard.process.Init)u8{
	const NoColour=Initialization.minimal.environ.getPosix("NO_COLOR");
	if((Initialization.minimal.environ.getPosix("CLICOLOR_FORCE")!=null)or(NoColour==null or NoColour.?.len==0)){
		//do something with color
	}
	else{
		//do something with label
	}

	if(Initialization.minimal.args.vector.len==1){
		return 0;
	}
	else{
		return 1;
	}
	//const Arguments=try Initialization.minimal.args.toSlice(Initialization.arena.allocator()) catch false;

	return 0;
}