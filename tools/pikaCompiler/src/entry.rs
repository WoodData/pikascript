use crate::compiler::*;
use crate::version_info::*;
use std::collections::HashSet;
use std::fs;
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;

fn collect_pyi_files(dir: &Path) -> std::io::Result<Vec<String>> {
    let mut result = Vec::new();
    if dir.is_dir() {
        for entry in fs::read_dir(dir)? {
            let entry = entry?;
            let path = entry.path();
            if path.is_file() && path.extension() == Some(std::ffi::OsStr::new("pyi")) {
                if let Some(file_name) = path.file_name() {
                    if let Some(file_name_str) = file_name.to_str() {
                        // remove extension
                        let file_name_str = file_name_str.split(".").collect::<Vec<&str>>()[0];
                        result.push(file_name_str.to_string());
                    }
                }
            }
        }
    }
    Ok(result)
}

pub fn pika_compiler_entry() {
    std::env::set_var("RUST_BACKTRACE", "1");
    let head_info = "/*
 * [Warning!] This file is auto-generated by pika compiler.
 * Do not edit it manually.
 * The source code is *.pyi file.
 * More details: 
 * English Doc:
 * https://pikadoc-en.readthedocs.io/en/latest/PikaScript%20%E6%A8%A1%E5%9D%97%E6%A6%82%E8%BF%B0.html
 * Chinese Doc:
 * http://pikapython.com/doc/PikaScript%20%E6%A8%A1%E5%9D%97%E6%A6%82%E8%BF%B0.html
 */

";

    /* new a version_info object */
    println!("(pikapython) packages installed:");
    let mut version_info = VersionInfo::new();
    version_info = VersionInfo::analyse_file(version_info, String::from("requestment.txt"));
    println!();

    println!("(pikapython) pika compiler:");
    /* new a compiler, sellect to path */
    let mut compiler = Compiler::new(String::from(""), String::from("pikascript-api/"));

    /* analyse file begin with main.py */
    compiler = Compiler::analyse_py_package_main(compiler, String::from("main"));
    /*
       Compile packages in requestment.txt, solve the packages
       as the top packages.
    */
    // Generate a list of module names

    let mut module_list = VersionInfo::new();
    module_list.plot = false;
    // check module_list.txt exists
    if Path::new("module_list.txt").exists() {
        module_list = VersionInfo::analyse_file(module_list, String::from("module_list.txt"));
    } else {
        module_list = VersionInfo::analyse_file(module_list, String::from("requestment.txt"));
    }
    let mut module_names: Vec<String> = module_list
        .package_list
        .iter()
        .filter_map(|(package_name, _)| {
            // skip pikascript-core
            if package_name == "pikascript-core" {
                None
            } else {
                Some(package_name.clone())
            }
        })
        .collect();

    // Add "main" to the start of the module_names
    module_names.insert(0, "main".to_string());

    // Process each module
    for module_name in &module_names {
        compiler = Compiler::import_module(compiler, module_name.clone());
    }
    compiler = Compiler::import_module(compiler, "PikaStdLib".to_string());
    compiler = Compiler::import_module(compiler, "PikaStdData".to_string());
    compiler = Compiler::import_module(compiler, "PikaDebug".to_string());
    compiler = Compiler::import_module(compiler, "builtins".to_string());

    /* search all *.pyi file and analyse */
    let dir = Path::new(".");
    let pyi_files = collect_pyi_files(&dir);
    for file in pyi_files.unwrap() {
        // skip PikaObj
        if file == "PikaObj" {
            continue;
        }
        // just generate *.h to support -lib compile
        compiler = Compiler::analyse_c_package_inner(compiler, file);
    }
    /* clean pikascript-api/ */
    compiler.clean_path();

    // println!();

    /* write the infomatrion to compiler-info */
    // let mut compiler_info_file =
    //     File::create(format!("{}compiler-info.txt", compiler.dist_path)).unwrap();
    // let compiler_info = format!("{:?}", compiler);
    // compiler_info_file.write(compiler_info.as_bytes()).unwrap();

    /* make the -api.c file for each python class */

    let api_file_path = format!("{}__pikaBinding.c", compiler.dist_path);
    let mut f = File::create(api_file_path).unwrap();
    f.write(head_info.as_bytes()).unwrap();
    /* create include for calsses */
    let mut written_includes = HashSet::new();

    f.write("#include <stdio.h>\n".as_bytes()).unwrap();
    f.write("#include <stdlib.h>\n".as_bytes()).unwrap();
    f.write("#include \"BaseObj.h\"\n".as_bytes()).unwrap();
    written_includes.insert(String::from("#include \"BaseObj.h\"\n"));

    for (_, class_info) in compiler.class_list.iter() {
        let include = class_info.include();

        // 将字符串按换行符分割成多个 include
        let includes: Vec<&str> = include.split('\n').collect();

        for include in includes {
            // 如果这个 include 之前没有被写入过
            if !written_includes.contains(include) {
                // 写入文件
                f.write(format!("{}\n", include).as_bytes()).unwrap();
                // println!("{} has been inserted", include);
                written_includes.insert(include.to_string());
            } else {
                // println!("{} has been written, skip", include);
            }
        }
    }

    f.write("\n".as_bytes()).unwrap();

    for (_, class_info) in compiler.class_list.iter() {
        if class_info.is_package && !class_info.is_top {
            continue;
        }
        /* get module_name */
        let mut module_name = class_info.this_file_name.clone();
        if module_name == "" {
            module_name = class_info.this_class_name.clone();
        }
        /* create module control macro */
        let module_define = format!(
            "#ifndef PIKA_MODULE_{}_DISABLE\n",
            module_name.to_ascii_uppercase()
        );
        f.write(module_define.as_bytes()).unwrap();
        /* create method api function */
        f.write(class_info.method_api_fn().as_bytes()).unwrap();
        /* create new classs function */
        f.write(class_info.new_class_fn().as_bytes()).unwrap();
        /* create contruactor */
        if !class_info.is_package {
            f.write("\n".as_bytes()).unwrap();
            let name = String::from(class_info.this_class_name.to_string());
            f.write(format!("Arg *{}(PikaObj *self){{\n", &name).as_bytes())
                .unwrap();
            f.write(format!("    return obj_newObjInPackage(New_{});\n", &name).as_bytes())
                .unwrap();
            f.write("}\n".as_bytes()).unwrap();
        }
        f.write("#endif\n".as_bytes()).unwrap();
        f.write("\n".as_bytes()).unwrap();
    }
    drop(f);

    /* make the .h file for each python class */
    for (_, class_info) in compiler.class_list.iter() {
        let api_file_path = format!("{}{}.h", compiler.dist_path, class_info.this_class_name);
        let mut f = File::create(api_file_path).unwrap();
        f.write(head_info.as_bytes()).unwrap();
        f.write(format!("#ifndef __{}__H\n", class_info.this_class_name).as_bytes())
            .unwrap();
        f.write(format!("#define __{}__H\n", class_info.this_class_name).as_bytes())
            .unwrap();
        f.write("#include <stdio.h>\n".as_bytes()).unwrap();
        f.write("#include <stdlib.h>\n".as_bytes()).unwrap();
        f.write("#include \"PikaObj.h\"\n".as_bytes()).unwrap();
        f.write("\n".as_bytes()).unwrap();
        let new_class_fn_declear = format!("{};\n", class_info.new_class_fn_name());
        f.write(new_class_fn_declear.as_bytes()).unwrap();
        f.write("\n".as_bytes()).unwrap();
        f.write(class_info.method_impl_declear().as_bytes())
            .unwrap();
        f.write("\n".as_bytes()).unwrap();
        f.write("#endif\n".as_bytes()).unwrap();
        drop(f);
    }
    /* make the pikascript.c */
    let api_file_path = format!("{}pikaScript.c", compiler.dist_path);
    let mut f = File::create(api_file_path).unwrap();
    /* add head */
    f.write(head_info.as_bytes()).unwrap();
    /* add include */
    f.write("#include \"PikaMain.h\"\n".as_bytes()).unwrap();
    f.write("#include <stdio.h>\n".as_bytes()).unwrap();
    f.write("#include <stdlib.h>\n".as_bytes()).unwrap();
    f.write("\n".as_bytes()).unwrap();
    /* get script from main.py */
    let pika_main = compiler
        .class_list
        .get_mut(&"PikaMain".to_string())
        .unwrap();
    /* make the pikascript.c */
    f.write(pika_main.script_fn(version_info).as_bytes())
        .unwrap();
    drop(f);

    /* make the pikascript.h */
    let api_file_path = format!("{}pikaScript.h", compiler.dist_path);
    let mut f = File::create(api_file_path).unwrap();
    f.write("/* ******************************** */\n".as_bytes())
        .unwrap();
    f.write("/* Warning! Don't modify this file! */\n".as_bytes())
        .unwrap();
    f.write("/* ******************************** */\n".as_bytes())
        .unwrap();
    f.write(format!("#ifndef __{}__H\n", "pikaScript").as_bytes())
        .unwrap();
    f.write(format!("#define __{}__H\n", "pikaScript").as_bytes())
        .unwrap();
    f.write("#include <stdio.h>\n".as_bytes()).unwrap();
    f.write("#include <stdlib.h>\n".as_bytes()).unwrap();
    f.write("#include \"PikaObj.h\"\n".as_bytes()).unwrap();
    f.write("#include \"PikaMain.h\"\n".as_bytes()).unwrap();
    f.write("\n".as_bytes()).unwrap();
    f.write("PikaObj * pikaPythonInit(void);\n".as_bytes())
        .unwrap();
    f.write("\n".as_bytes()).unwrap();
    f.write("#endif\n".as_bytes()).unwrap();
    drop(f);

    // Write to a file
    let file_name = format!("{}module_list_default.txt", compiler.dist_path);
    let mut f = File::create(file_name).expect("Unable to create file");
    let content = module_names.join("\n") + "\n";
    f.write_all(content.as_bytes())
        .expect("Unable to write data");
    drop(f);
    compiler.reuse_old_file();
}
