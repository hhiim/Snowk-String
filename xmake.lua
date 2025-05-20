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

-- 打包
target("Snowk-String")
    set_arch("x64")
    set_default(true)
    set_kind("headeronly")
    add_headerfiles("include/**.h")
    add_headerfiles("include/**.hpp")
    
    set_basename("Snowk-String")
    add_includedirs("include", {public = true})
    set_toolchains("gcc-14")
target_end()

-- 手动测试
target("main")
    add_files("src/*.cpp")
    add_deps("Snowk-String")
    set_default(false)
    on_install(function (target) end)
target_end()

-- 用例测试
target("Snowk-String-test")
    set_default(false)
    set_arch("x64")
    set_basename("String")
    set_toolchains("gcc-14")
    set_kind("binary")
    set_targetdir("bin/x64/test")
    add_files("test/*.cpp")

    add_deps("Snowk-String")
    set_default(false)
    on_install(function (target) end)
target_end()


