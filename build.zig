const Standard:type=@import("std");

pub fn build(Build:*Standard.Build)void{
    const Target=Build.standardTargetOptions(.{});
    const Optimization=Build.standardOptimizeOption(.{
        .preferred_optimize_mode=.ReleaseFast,
    });
    const Chameleon=Build.dependency("chameleon",.{});
    const Compiler=Build.addExecutable(.{
        .name="CatigerCompiler",
        .root_module=Build.createModule(.{
            .root_source_file=Build.path("Compiler/Main.zig"),
            .target=Target,
            .optimize=Optimization,
            .imports=&.{
                .{.name="Chameleon",.module=Chameleon.module("chameleon")}
            }
            }),
    });
    Build.installArtifact(Compiler);
}