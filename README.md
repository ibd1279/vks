# vks
vks is a Vulkan bridge for Go. The header generator folder contains the code that
is used to generate the vulkan bindings. It woks similar to c-for-go, but it uses
the vulkan specification published as XML to generate the output bindings.

## Memory management
Unlike c-for-go, this wrapper does very little memory management for you. Most of
the logic is for converting from Cgo types into go types. Ensuring that C
memory is allocated and released is up to the program, not to this binding.

A helper method for CPtr() exists that makes it easier to copy the structure to
C. The program must call free on that structure to release the C memory. It
uses the Function options pattern to for building the structs. An
implementation of a autorelease pool is included as well to reduce the amount
of deferred Free() calls in your code.

## Completeness
A list of the enabled features and extensions is available in the [vkxml.yml](https://github.com/ibd1279/vks/blob/main/vkxml.yml) file. 

The project doesn't do much to make the Vulkan APIs much easier to use. Commands
and types have the Vk prefix stripped, but not the constants. Facade objects are
used to access the methods, as they combine the function poitners with the handle
required to invoke the function. Most of this was on purpose to make it
familiar for anyone coming from the specification.

## Example application
An [example application](https://github.com/ibd1279/vks/blob/main/example/main.go) is included in the code base. It isn't trying to be a complete application. It mostly demonstrates how some of the foundational aspects of Vulkan are done with this binding: Creating structures, creating an instance, and using pNext. More complete examples are available at [ibd1279/vks-examples](https://github.com/ibd1279/vks-examples).

## Expected usage.

The expected usage is to configure a vkxml.yml file for a project, and use
the header-generator program to expose the vulkan functionality you need to
your program.

That being said, I intend to keep the (in my opinion) most common
extensions enabled and available for vks, partially for testing, and partially
as a living example of expected usage.

## Next steps

Listed in no particular order:
* add WSI support, and putting certain extension data in different files (with
  different build tags).
* Put better documentation into the output. Hoping to find a way to pull some of the
  Vulkan docs into the file.
