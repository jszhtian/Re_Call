#encoding :utf-8
module RORL
    module_function
    ec = Encoding::Converter.new("utf-8", "gbk")
    path = ec.convert(File.expand_path("rorl.dll"))
    RL = Fiddle.dlopen(path)
    ROR = Fiddle::Function.new(
        RL["_ror"],
        [Fiddle::TYPE_VOIDP,
         Fiddle::TYPE_INT],
         Fiddle::TYPE_VOID
    )
    ROL = Fiddle::Function.new(
    RL["_rol"],
    [Fiddle::TYPE_VOIDP,
     Fiddle::TYPE_INT],
     Fiddle::TYPE_VOID
    )
    def ror(data, size)
        ROR.call(data, size)
    end
    def rol(data, size)
        ROL.call(data, size)
    end
end