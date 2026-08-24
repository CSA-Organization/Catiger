const std = @import("std");
pub const ComptimeChameleon = @import("api/Comptime.zig");
pub const RuntimeChameleon = @import("api/Runtime.zig");
pub const HexColors = @import("colors.zig");

pub fn initComptime() ComptimeChameleon {
    return .{};
}

const Config = struct {
    allocator: std.mem.Allocator,
    no_color: bool = true,
    io: std.Io,
};

pub fn initRuntime(config: Config) RuntimeChameleon {
    return .{
        .allocator = config.allocator,
        .no_color = config.no_color,
        .io = config.io,
    };
}

test {
    std.testing.refAllDecls(@This());
}
