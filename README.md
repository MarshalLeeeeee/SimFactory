# SimFactory
A lib provides easy simulation backboned by DirectX

![frame](doc/SimFactory.svg)


## Introduction
This project provides a simple backbone of simulation. Some concepts are introduced as follows.
 - App: ```SimApp``` handles some basic function of the application, incl. managing window, initializing DirectX basic COMs, updating and rendereing in frame, signal handling, etc.  
 - Case: ```SimCase``` provides the basic structure of a simulation case. A case holds UI (backboned by ImGUI) and entities. Basicly, if one wants to implement a specific simulation case, define a new case that inherits from SimCase, like ```FadingTriangleCase```.
 - Entity: ```Entity``` represents a simulation object. A simulation object holds simulation properties, like position, rotation, speed, etc. To be displayed in window, it should owns a model.
 - Model: ```Model``` is consist of meshes. Model is loaded in an asynchronized way from ```model/xxx.xml``` which refers vertex data from ```vertex/xxx.xml``` and defines mesh list with index data, primitive type, vertex shader and index shader.
 - Mesh: ```Mesh``` refers vertex buffer, index buffer, vertex material, index material, vertex material buffer, index material buffer.
 - Material: ```VertexMaterial``` encasulates vertex layout and vertex shader. ```PixelMaterial``` encapsulates pixel shader. Both ```VertexMaterialBuffer``` and ```PixelMaterialBuffer``` encapsulate constant buffers for each. Vertex layout in defined in ```TypeUtil```, if one introduces a new vertex layout, one should implement ```parse``` method and modify ```loadVertexFromFile```. If one introduces a new shader, one should implement new Material buffer type like ```VMBPosColorRaw``` and ```PMBPosColorRaw```.
 - Vertex Buffer: ```VertexBuffer``` encapsulates vertex buffer (immutable) and its data.
 - Index Buffer: ```IndexBuffer``` encapsulated index buffer (immutable) and its data.

This project has the following singletons.
 - Thread pool: ```Dispatcher``` is a thread pool. Main thread can submit functions to it.
 - Main thread callback: ```MainthreadCallbackHub``` is a hub collecting callbacks from sub therads. Normally, only sub thread can sumbit functions to it.
 - Logger: ```Logger``` is a multithread log recorder. Main thread submit log task and sub thread append log to file stream. Dynamic count and type of arguments are supported.
 - Bunch of resource pools:
   - ```VertexBufferPool``` manages ```VertexBuffer``` hashed by same vertex filename.
   - ```IndexBufferPool``` manages ```IndexBuffer``` hashed by same mesh name.
   - ```VertexMaterialPool``` manages ```VertexMaterial``` hashed by same vertex shader filename.
   - ```IndexMaterialPool``` manages ```IndexMaterial``` hashed by same pixel shader filename.