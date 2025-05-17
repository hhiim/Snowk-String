add_includedirs("include")
add_includedirs("include/encode")
add_includedirs("include/endian")
add_includedirs("include/instances")
add_includedirs("include/metaprogram")
set_languages("c23", "c++23")

add_cxxflags(
    "-g","-w","-fpermissive", "-O0",
    {force = true}
)

target("Snowk-String")
    set_default(true)
    set_kind("headeronly")
    set_arch("x64")
    set_basename("Snowk-String")
    add_includedirs("include", {public = true})
    set_toolchains("gcc-14")
    set_targetdir("bin/x64")
target_end()


target("main")
    add_files("src/*.cpp")
    add_deps("Snowk-String")           -- 依赖主库
    set_default(false)                 -- 不默认构建
    on_install(function (target) end)  -- 空安装函数，不安装
target_end()

target("Snowk-String-test")
    set_default(false)
    set_arch("x64")
    set_basename("String")
    set_toolchains("gcc-14")
    set_kind("binary")
    set_targetdir("bin/x64/test")
    add_files("test/*.cpp")

    add_deps("Snowk-String")           -- 依赖主库
    set_default(false)                 -- 不默认构建
    on_install(function (target) end)  -- 空安装函数，不安装
target_end()


