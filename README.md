<h1> Setup the Project <h1>
<p>
  Ensure that you have installed the vcpkg package manager is installed in your computer and also awhere 
  it located this will required in during project setup
</p>

<hr>
<h2> Steps</h2>
<ol>
  <li>Install the vcpkg in pc and also add the environment variable locaiton in the path.</li>
  <li>Then, navigate insside in the project folder type: <code>vcpkg install</code></li>
  <li>Then, type: 
    <code>
      cmake --fresh -S . -B build  -DCMAKE_TOOLCHAIN_FILE=C:/<i><u>path_where_vkpkg_installed</i></u>/vcpkg/scripts/buildsystems/vcpkg.cmake
      -DVCPKG_TARGET_TRIPLET=x64-windows 
      <code>
  </li>
  <li>Then, type: <code>cmake --build build <code> or open the solution the vs2022 set the start up as the current project then run(debug/relase)</li>
  <li>Then, will use the executable or do whatever</li>  
</ol>
