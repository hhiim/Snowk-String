add_files("src/*.cpp")
add_includedirs("include")
add_includedirs("include/encode")
add_includedirs("include/endian")
add_includedirs("include/instances")

set_languages("c23", "c++20")
add_cxxflags("-g","-Wall", {force = true} )



toolchain("mips")
    set_kind("standalone")
    -- 设置编译器和工具链
    set_toolset("cxx", "mips-linux-gnu-g++-14")
    set_toolset("cc", "mips-linux-gnu-gcc-14")
    set_toolset("ar", "mips-linux-gnu-ar")
    set_toolset("as", "mips-linux-gnu-as")
    set_toolset("ranlib", "mips-linux-gnu-ranlib")
    set_toolset("strip", "mips-linux-gnu-strip")
    set_toolset("ld", "mips-linux-gnu-g++-14")
    -- 设置库和头文件路径
    add_linkdirs("/usr/mips-linux-gnu/lib/")
    add_includedirs("/usr/mips-linux-gnu/include")
    add_includedirs("/usr/mips-linux-gnu/include/c++/14/")

    -- 设置编译器选项
    add_cxxflags("-static")
    add_ldflags("-static", {force = true})
toolchain_end()



target("mips")
    set_arch("mips")
    set_basename("String")
    set_toolchains("mips")
    set_kind("binary")
    set_targetdir("bin/mips")
target_end()


target("x64")
    set_arch("x64")
    set_basename("String")
    set_toolchains("gcc")
    set_kind("binary")
    set_targetdir("bin/x64")
target_end()




