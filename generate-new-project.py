import uuid
import argparse
import os

# GUID for the Common.vcxproj which all projects depend on.
COMMON_PROJECT_IDENTIFIER_GUID = "19FF8CFD-CA0F-485A-8217-D86A1A8184AF"

# GUID defined by Microsoft for Project types.
PROJECT_TYPE_IDENTIFIER_GUID = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"

# GUID defined by Microsoft for source folder
SOURCE_FOLDER_GUID = "4FC737F1-C7A5-4376-A066-2A32D752A2FF"

# GUID defined by Microsoft for header folder
HEADER_FOLDER_GUID = "93995380-89BD-4b04-88EB-625FBE52EBFB"

# GUID defined by Microsoft for resource folder
RESOURCE_FOLDER_GUID = "67DA6AB6-F800-4c08-8B7A-83BB121AAD01"

# A project must have a GUID. The .sln uses it to identify its associated
# project.
NEW_PROJECT_GENERATED_GUID = str(uuid.uuid4()).upper()

SLN_PATH = "leet-code.sln"

# Tag that signifies the beginning of the Global section in a .sln
SLN_SECTION_TAG_GLOBAL_BEGIN = "Global"

# Tag that signifies the beginning of the platform section of a .sln
SLN_SECTION_TAG_PROJECT_PLATFORMS_BEGIN = "ProjectConfigurationPlatforms"

# Generic end of any global section. Its important to associate this with a
# specific beginning tag.
SLN_SECTION_TAG_PROJECT_PLATFORMS_END = "EndGlobalSection"

# vcxproj fields

# Path to the debug props file.
DEBUG_PROPS_FILENAME = "../props/debug.props"

# Path to the release props file.
RELEASE_PROPS_FILENAME = "../props/release.props"

# Default stub cpp for the new project.
MAIN_CPP_CONTENTS = '''#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/Log.h"
#include "common/Timer.h"

#include <algorithm>
#include <assert.h>
#include <vector>
\nSCENARIO("<brief problem description>")
{
\tGIVEN("<the current condition>")
\t{
\t\tREQUIRE(true);
\t}
}
'''


class MissingBeginTagException(Exception):
    """
    Exception raised when attempting to insert fields, such as into
    a .sln file, without specifying the begin_tag of the section it should
    be inserted above.

    """
    def __init__(self, message="A begin_tag at minimum is required for text"
                               "insertion."):
        self.message = message
        super().__init__(self.message)


def find_section_tags_index_from_sln(target_section, sln_content):
    """
    When we want to insert fields into the .sln, we need to find
    the appropriate section. Some section tags are ambigious.
    For example:
    
    begin_tags denote the start of a section:
        GlobalSection(SolutionConfigurationPlatforms)
        GlobalSection(ProjectConfigurationPlatforms)
        GlobalSection(NestedProjects)
    
    All of those sections however, are closed with the same end_tag:
        EndGlobalSection

    So, at a minimum, we always need at least a begin_tag. With that, we
    can at least find a unique position to insert above.

    But if we want to insert at the bottom of a section, we must specify
    both the beginning and end tag, so that we can find the associated
    end of the desired section.

    Args:
        target_section (tuple): A tuple containing (begin_tag, end_tag).
            end_tag is allowed to be None. Look to
            SLN_SECTION_TAG_PROJECT_PLATFORMS_BEGIN and
            SLN_SECTION_TAG_PROJECT_PLATFORMS_END and their usage as examples.

    Returns:
        tuple: A tuple containing the line index in the .sln of each tag.
    """
    begin_tag, end_tag = target_section

    if begin_tag is None:
        raise MissingBeginTagException()

    begin_index, end_index = -1, -1

    for i, line in enumerate(sln_content):

        # We've found the target begin tag
        if begin_tag in line:
            begin_index = i
            if end_tag is None:
                return (begin_index, end_index)

        # The end_tag is generic and could appear multiple times. So we need
        # to make sure that its the one associated with our begin tag.
        if begin_index != -1 and line.strip() == end_tag:
            end_index = i
            return (begin_index, end_index)

    return (begin_index, end_index)


def copy_sln_file(source_path, destination_path):
    """
    Copy the contents of the source .sln file to the destination .sln file.
    This is so we always have a backup.
    Args:
        source_path (str): The path to the source .sln file.
        destination_path (str): The path to the destination .sln file.

    Returns:
        bool: True if the copy was successful, False otherwise.
    """
    try:
        with open(source_path, "r", encoding="utf-8") as source_file:
            sln_content = source_file.read()

        with open(destination_path, "w", encoding="utf-8") as destination_file:
            destination_file.write(sln_content)

        return True

    except Exception as e_c:
        print(f"Error copying .sln file: {e_c}")
        return False


def generate_configuration_set(generated_guid):
    """
    Generate a configuration set for a project using the provided GUID.

    Args:
        generated_guid (str): The generated GUID for the project.

    Returns:
        str: A string containing the configuration set.
    """
    configuration_set = [
        f"\t\t{{{generated_guid}}}.Debug|x64.ActiveCfg = Debug|x64",
        f"\t\t{{{generated_guid}}}.Debug|x64.Build.0 = Debug|x64",
        f"\t\t{{{generated_guid}}}.Release|x64.ActiveCfg = Release|x64",
        f"\t\t{{{generated_guid}}}.Release|x64.Build.0 = Release|x64",
    ]

    # Join the set with "\n"s and add one to the back.
    return "\n".join(configuration_set) + "\n"


def generate_project_entry(project_name):
    """
    Generate a project entry string for a Visual Studio Solution (.sln) file.

    Args:
        project_name (str): The name of the project.

    Returns:
        str: A project entry string in the specified format.
    """
    # Generate a new GUID

    # Format the project entry string
    project_entry = (
        f"Project(\"{{{PROJECT_TYPE_IDENTIFIER_GUID}}}\")"
        f" = \"{project_name}\", "
        f"\"{project_name}\\{project_name}.vcxproj\", "
        f"\"{{{NEW_PROJECT_GENERATED_GUID}}}\""
    )
    project_entry += (
        "\n\tProjectSection(ProjectDependencies) = postProject"
        f"\n\t\t{{{COMMON_PROJECT_IDENTIFIER_GUID}}}"
        f" = {{{COMMON_PROJECT_IDENTIFIER_GUID}}}"
        "\n\tEndProjectSection"
        "\nEndProject"
    )

    return project_entry + "\n"


def update_sln_with_configuration_set(sln_content, config_set):
    """
    Update a .sln file with the provided configuration set.

    Args:
        sln_path (str): The path to the .sln file.
        configuration_set (str): The configuration set generated for the
        project.

    Returns:
        str: The updated content of the .sln file
    """

    # The section we want to insert in between.
    section_tags = (
        SLN_SECTION_TAG_PROJECT_PLATFORMS_BEGIN,
        SLN_SECTION_TAG_PROJECT_PLATFORMS_END
    )

    # We don't care about the begin_index here.
    _, end_index = find_section_tags_index_from_sln(section_tags,
                                                    sln_content)

    # We want to insert above the end_index. That will be the bottom of the
    # section.
    if end_index != -1:
        updated_sln_content = sln_content
        updated_sln_content.insert(end_index, config_set)

        return updated_sln_content

    raise ValueError("ProjectConfigurationPlatforms GlobalSection not found in"
                     "the .sln file")


def update_sln_with_project_files(sln_content, project_entry):
    """
    Update the .sln file content to include the newly generated project entry

    Args:
        sln_content (str): The content of the .sln file to be updated.
        project_entry (str): The project entry to be added to the .sln
        file content.

    Returns:
        str: The updated content of the .sln file after including the project
        entry.

    Raises:
        ValueError: If the global section is not found in the .sln file.
    """
    section_tags = (
        SLN_SECTION_TAG_GLOBAL_BEGIN,
        None
    )

    begin_index, _ = find_section_tags_index_from_sln(section_tags,
                                                      sln_content)
    if begin_index != -1:
        updated_sln_content = sln_content
        updated_sln_content.insert(begin_index, project_entry)

        return updated_sln_content

    raise ValueError("Global section not found in the .sln file")


def generate_vcxproj(project_name):
    """
    Generates a vcxproj file based on the name.

    Args:
        project_name (str): The name that will be hard associated with the
        project generated

    Returns:
        None: we simply write the file.
    """
    vcxproj_template = f'''<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <ProjectGuid>{NEW_PROJECT_GENERATED_GUID}</ProjectGuid>
    <Keyword>MakeFileProj</Keyword>
    <RootNamespace>{project_name}</RootNamespace>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <PlatformToolset>v143</PlatformToolset>
    <UseDebugLibraries>true</UseDebugLibraries>
    <CharacterSet>MultiByte</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <PlatformToolset>v143</PlatformToolset>
    <UseDebugLibraries>false</UseDebugLibraries>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>MultiByte</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="Shared">
  </ImportGroup>
  <ImportGroup Label="PropertySheets">
    <Import Project="{DEBUG_PROPS_FILENAME}" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" />
    <Import Project="{RELEASE_PROPS_FILENAME}" Condition="'$(Configuration)|$(Platform)'=='Release|x64'" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup />
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <LanguageStandard>stdcpp20</LanguageStandard>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <LanguageStandard>stdcpp20</LanguageStandard>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClCompile Include="main.cpp" />
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>
'''
    project_directory = os.path.join(os.getcwd(), project_name)
    vcxproj_file_path = os.path.join(project_directory, f'{project_name}.vcxproj')
    with open(vcxproj_file_path, 'w') as vcxproj_file:
        vcxproj_file.write(vcxproj_template)


def generate_vcxproj_filters(project_name):
    """
    Generates a vcxproj.filter file based on the name.

    Args:
        project_name (str): The name that will be hard associated with the
        filters generated

    Returns:
        None: we simply write the file.
    """

    filters_template = f'''<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup>
    <Filter Include="Source Files">
      <UniqueIdentifier>{SOURCE_FOLDER_GUID}</UniqueIdentifier>
    </Filter>
    <Filter Include="Header Files">
      <UniqueIdentifier>{HEADER_FOLDER_GUID}</UniqueIdentifier>
    </Filter>
    <Filter Include="Resource Files">
      <UniqueIdentifier>{RESOURCE_FOLDER_GUID}</UniqueIdentifier>
    </Filter>
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include="main.cpp">
      <Filter>Source Files</Filter>
    </ClCompile>
    <!-- Add more ClCompile entries for other source files as needed -->
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="common/Log.h">
      <Filter>Header Files</Filter>
    </ClInclude>
    <!-- Add more ClInclude entries for other header files as needed -->
  </ItemGroup>
  <ItemGroup>
    <!-- Add more ResourceCompile entries for resource files as needed -->
  </ItemGroup>
</Project>
'''

    # TODO: Maybe put writing to the project path into its own function...
    project_directory = os.path.join(os.getcwd(), project_name)
    filters_file_path = os.path.join(project_directory, f'{project_name}.vcxproj.filters')
    with open(filters_file_path, 'w') as filters_file:
        filters_file.write(filters_template)


def generate_main_cpp(project_directory):
    """
    Generates a template cpp file based in the project directory.

    Args:
        project_directory (str): The directory that the cpp file will
        be written to.

    Returns:
        None: we simply write the file.
    """

    main_cpp_file_path = os.path.join(project_directory, 'main.cpp')
    with open(main_cpp_file_path, 'w') as main_cpp_file:
        main_cpp_file.write(MAIN_CPP_CONTENTS)


def main():
    """
    Given a project name, this script will generate a project and all of
    its associated files, update the .sln file, and generate a template
    cpp file.
    """

    parser = argparse.ArgumentParser(description="Generate project files"
                                     " and update solution files")

    parser.add_argument("project_name", help="Name of the project to generate")

    args = parser.parse_args()
    project_name = args.project_name
    project_directory = os.path.join(os.getcwd(), project_name)
    os.makedirs(project_directory, exist_ok=True)

    print("Creating backup to leet-code.sln...")
    copy_sln_file("leet-code.sln", "leet-code-backup.sln")
    print("leet-code-backup.sln created...")

    sln_content = []
    with open(SLN_PATH, 'r', encoding="utf8") as sln_file:
        sln_content = sln_file.readlines()

    print("Updating leet-code.sln project configurations...")
    config_set = generate_configuration_set(NEW_PROJECT_GENERATED_GUID)
    # print(config_set)

    updated_sln_content = update_sln_with_configuration_set(sln_content,
                                                            config_set)

    print("Updating leet-code.sln project list...")
    project_entry_string = generate_project_entry(project_name)
    # print(project_entry_string)

    update_sln_with_project_files(updated_sln_content, project_entry_string)

    with open(SLN_PATH, 'w', encoding="utf8") as sln_file:
        sln_file.writelines(updated_sln_content)

    print("Creating new project files...")
    generate_vcxproj(project_name)
    generate_vcxproj_filters(project_name)
    generate_main_cpp(project_directory)

    print("Done.")


if __name__ == "__main__":
    main()
