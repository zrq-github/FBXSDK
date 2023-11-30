#include <fbxsdk.h>
#include "FbxSdkHelp.h"

FbxSdkHelp::FbxSdkHelp()
{
    FbxManager* lSdkManager = FbxManager::Create();
    m_pSdkManager = lSdkManager;
    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene_fbxSdk");
    m_pMyScene = lScene;
}

FbxSdkHelp::~FbxSdkHelp()
{
    m_pSdkManager->Destroy();
}

void FbxSdkHelp::RunMyFirstProgrammer()
{
    // Change the following filename to a suitable filename value.
    const char* lFilename = R"(C:\Users\zrq\Desktop\FBX\FbxTest\rvt_square_property.fbx)";

    // Initialize the SDK manager. This object handles all our memory management.
    FbxManager* lSdkManager = FbxManager::Create();

    // Create the IO settings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file is imported; so get rid of the importer.
    lImporter->Destroy();

    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.
    FbxNode* lRootNode = lScene->GetRootNode();
    if (lRootNode) {
        for (int i = 0; i < lRootNode->GetChildCount(); i++)
            PrintNode(lRootNode->GetChild(i));
    }
    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();
}

void FbxSdkHelp::RunAddFbxNode()
{
    // Create the scene.
    FbxScene* pScene = FbxScene::Create(m_pSdkManager, "Scene Name");
    // Get the root node of the scene.
    FbxNode* pRootNode = pScene->GetRootNode();
    // Create a child node.
    FbxNode* pChild = FbxNode::Create(pScene, "child");
    // Add the child to the root node.
    pRootNode->AddChild(pChild);

    ExportScene(pScene);
}

void FbxSdkHelp::RunFbxProperty()
{
    FbxScene* pScene = FbxScene::Create(m_pSdkManager, "Scene FbxProperty");
    FbxNode* pRootNode = pScene->GetRootNode();
    FbxNode* pNode = FbxNode::Create(pScene, "node_property");
    pRootNode->AddChild(pNode);

    FbxProperty p = FbxProperty::Create(pScene,FbxDouble3DT, "Vector3Property");
    p.Set(FbxDouble3(1.1, 2.2, 3.3));

    //FbxObjectMetaData* pFamilyMetaData = FbxObjectMetaData::Create(pScene, "Family");
    //FbxProperty::Create(pFamilyMetaData, FbxStringDT, "Level", "Level").Set(FbxString("Family"));
    //FbxProperty::Create(pFamilyMetaData, FbxStringDT, "Type", "Type").Set(FbxString("Wall"));
    //FbxProperty::Create(pFamilyMetaData, FbxFloatDT, "Width", "Width").Set(10.0f);
    //FbxProperty::Create(pFamilyMetaData, FbxDoubleDT, "Weight", "Weight").Set(25.0);
    //FbxProperty::Create(pFamilyMetaData, FbxDoubleDT, "Cost", "Cost").Set(1.25);

    //FbxObjectMetaData* pTypeMetaData = FbxCast<FbxObjectMetaData>(pFamilyMetaData->Clone(FbxObject::eReferenceClone, pScene));

    //pTypeMetaData->SetName("Type");

    //// On this level we'll just override two properties
    //pTypeMetaData->FindProperty("Cost").Set(2500.0);
    //pTypeMetaData->FindProperty("Level").Set(FbxString("Type"));

    //FbxObjectMetaData* pInstanceMetaData = FbxCast<FbxObjectMetaData>(pTypeMetaData->Clone(FbxObject::eReferenceClone, pScene));

    //pInstanceMetaData->SetName("Instance");

    //// And on this level, we'll go in and add a brand new property, too.
    //FbxProperty::Create(pInstanceMetaData, FbxStringDT, "Sku", "Sku#").Set(FbxString("143914-10"));
    //pInstanceMetaData->FindProperty("Width").Set(1100.50f);
    //pInstanceMetaData->FindProperty("Type").Set(FbxString("Super Heavy Duty Wall"));
    //pInstanceMetaData->FindProperty("Level").Set(FbxString("Instance"));

    //pNode->ConnectSrcProperty(p);

    ExportScene(pScene);
}

void FbxSdkHelp::RunCreateMeshes()
{
    FbxScene* pScene = FbxScene::Create(m_pSdkManager, "Scene CreateMeshes");
    FbxNode* pRootNode = pScene->GetRootNode();
    FbxNode* lMeshNode = FbxNode::Create(pScene, "meshNode");
    pRootNode->AddChild(lMeshNode);

    // Create a mesh.
    FbxMesh* lMesh = FbxMesh::Create(pScene, "mesh");
    // Set the node attribute of the mesh node.
    lMeshNode->SetNodeAttribute(lMesh);

    CreateOctahedron(lMesh);
    ExportScene(pScene);
    return;

    // 设置顶点
    lMesh->InitControlPoints(4);
    FbxVector4* vertices = lMesh->GetControlPoints();
    vertices[0] = FbxVector4(-1.0, 0.0, 0.0);
    vertices[1] = FbxVector4(0.0, 1.0, 0.0);
    vertices[2] = FbxVector4(1.0, 0.0, 0.0);

    // 设置三角形
    lMesh->BeginPolygon();
    lMesh->AddPolygon(0);
    lMesh->AddPolygon(1);
    lMesh->AddPolygon(2);
    lMesh->EndPolygon();
    return;

    // Define the eight corners of the cube.
    // The cube spans from
    //    -50 to  50 along the X axis
    //      0 to 100 along the Y axis
    //    -50 to  50 along the Z axis
    FbxVector4 vertex0(-50, 0, 50);
    FbxVector4 vertex1(50, 0, 50);
    FbxVector4 vertex2(50, 100, 50);
    FbxVector4 vertex3(-50, 100, 50);
    FbxVector4 vertex4(-50, 0, -50);
    FbxVector4 vertex5(50, 0, -50);
    FbxVector4 vertex6(50, 100, -50);
    FbxVector4 vertex7(-50, 100, -50);

    // Initialize the control point array of the mesh.
    lMesh->InitControlPoints(24);
    FbxVector4* lControlPoints = lMesh->GetControlPoints();

    // Define each face of the cube.
    // Face 1
    lControlPoints[0] = vertex0;
    lControlPoints[1] = vertex1;
    lControlPoints[2] = vertex2;
    lControlPoints[3] = vertex3;
    // Face 2
    lControlPoints[4] = vertex1;
    lControlPoints[5] = vertex5;
    lControlPoints[6] = vertex6;
    lControlPoints[7] = vertex2;
    // Face 3
    lControlPoints[8] = vertex5;
    lControlPoints[9] = vertex4;
    lControlPoints[10] = vertex7;
    lControlPoints[11] = vertex6;
    // Face 4
    lControlPoints[12] = vertex4;
    lControlPoints[13] = vertex0;
    lControlPoints[14] = vertex3;
    lControlPoints[15] = vertex7;
    // Face 5
    lControlPoints[16] = vertex3;
    lControlPoints[17] = vertex2;
    lControlPoints[18] = vertex6;
    lControlPoints[19] = vertex7;
    // Face 6
    lControlPoints[20] = vertex1;
    lControlPoints[21] = vertex0;
    lControlPoints[22] = vertex4;
    lControlPoints[23] = vertex5;

    // Define normal vectors along each axis.
    FbxVector4 lNormalXPos(1, 0, 0);
    FbxVector4 lNormalXNeg(-1, 0, 0);
    FbxVector4 lNormalYPos(0, 1, 0);
    FbxVector4 lNormalYNeg(0, -1, 0);
    FbxVector4 lNormalZPos(0, 0, 1);
    FbxVector4 lNormalZNeg(0, 0, -1);

    // Create layer 0 for the mesh if it does not already exist.
    // This is where we will define our normals.
    auto lLayer = lMesh->GetLayer(0);
    if (lLayer == nullptr) {
        auto lLayerIndex = lMesh->CreateLayer();
        lLayer = lMesh->GetLayer(lLayerIndex);
    }

    // Create a normal layer.
    FbxLayerElementNormal* lLayerElementNormal = FbxLayerElementNormal::Create(lMesh, "");

    // Set its mapping mode to map each normal vector to each control point.
    lLayerElementNormal->SetMappingMode(FbxLayerElement::eByControlPoint);

    // Set the reference mode of so that the n'th element of the normal array maps to the n'th
    // element of the control point array.
    lLayerElementNormal->SetReferenceMode(FbxLayerElement::eDirect);

    // Assign the normal vectors in the same order the control points were defined for the mesh.
    // Face 1
    lLayerElementNormal->GetDirectArray().Add(lNormalZPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalZPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalZPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalZPos);
    // Face 2
    lLayerElementNormal->GetDirectArray().Add(lNormalXPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalXPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalXPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalXPos);
    // Face 3
    lLayerElementNormal->GetDirectArray().Add(lNormalZNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalZNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalZNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalZNeg);
    // Face 4
    lLayerElementNormal->GetDirectArray().Add(lNormalXNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalXNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalXNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalXNeg);
    // Face 5
    lLayerElementNormal->GetDirectArray().Add(lNormalYPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalYPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalYPos);
    lLayerElementNormal->GetDirectArray().Add(lNormalYPos);
    // Face 6
    lLayerElementNormal->GetDirectArray().Add(lNormalYNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalYNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalYNeg);
    lLayerElementNormal->GetDirectArray().Add(lNormalYNeg);

    // Finally, we set layer 0 of the mesh to the normal layer element.
    lLayer->SetNormals(lLayerElementNormal);
    ExportScene(pScene);
}

void FbxSdkHelp::ExportScene(FbxScene* pFbxScene, const char* lFilename)
{
    // Create the FBX SDK manager
    FbxManager* lSdkManager = FbxManager::Create();

    // Create an IOSettings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // ... Configure the FbxIOSettings object here ...

    // Create an exporter.
    FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");

    // Initialize the exporter.
    bool lExportStatus = lExporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());

    // Export the scene to the file.
    bool isExport = lExporter->Export(pFbxScene);

    // Destroy the exporter.
    lExporter->Destroy();
    lSdkManager->Destroy();
}

FbxNode* FbxSdkHelp::CreateFbxNode(FbxScene* pFbxScene)
{
    auto pRotNode = pFbxScene->GetRootNode();
    auto pChild = FbxNode::Create(pFbxScene, "child");
    pRotNode->AddChild(pChild);
    return pChild;
}

void FbxSdkHelp::CreateOctahedron(FbxMesh* pMesh)
{
    // 创建六面体的顶点和索引
    FbxVector4 vertices[12] = {
        FbxVector4(0, 0, 0),
        FbxVector4(10, 0, 0),
        FbxVector4(15, 5, 0),
        FbxVector4(10, 10, 0),
        FbxVector4(0, 10, 0),
        FbxVector4(-5, 5, 0),

        FbxVector4(0, 0, 30),
        FbxVector4(10, 0, 30),
        FbxVector4(15, 5, 30),
        FbxVector4(10, 10, 30),
        FbxVector4(0, 10, 30),
        FbxVector4(-5, 5, 30),
    };

    // 设置顶点和索引
    pMesh->InitControlPoints(12);
    for (int i = 0; i < 12; ++i) {
        pMesh->SetControlPointAt(vertices[i], i);
    }

    pMesh->BeginPolygon();
    pMesh->AddPolygon(0);
    pMesh->AddPolygon(1);
    pMesh->AddPolygon(2);
    pMesh->AddPolygon(3);
    pMesh->AddPolygon(4);
    pMesh->AddPolygon(5);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(6);
    pMesh->AddPolygon(7);
    pMesh->AddPolygon(8);
    pMesh->AddPolygon(9);
    pMesh->AddPolygon(10);
    pMesh->AddPolygon(11);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(0);
    pMesh->AddPolygon(1);
    pMesh->AddPolygon(7);
    pMesh->AddPolygon(6);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(1);
    pMesh->AddPolygon(2);
    pMesh->AddPolygon(8);
    pMesh->AddPolygon(7);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(2);
    pMesh->AddPolygon(3);
    pMesh->AddPolygon(9);
    pMesh->AddPolygon(8);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(3);
    pMesh->AddPolygon(4);
    pMesh->AddPolygon(10);
    pMesh->AddPolygon(9);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(4);
    pMesh->AddPolygon(5);
    pMesh->AddPolygon(11);
    pMesh->AddPolygon(10);
    pMesh->EndPolygon();

    pMesh->BeginPolygon();
    pMesh->AddPolygon(5);
    pMesh->AddPolygon(0);
    pMesh->AddPolygon(6);
    pMesh->AddPolygon(11);
    pMesh->EndPolygon();
}

void FbxSdkHelp::PrintTabs()
{
    for (int i = 0; i < numTabs; i++)
        printf("\t");
}

FbxString FbxSdkHelp::GetAttributeTypeName(FbxNodeAttribute::EType type)
{
    switch (type)
    {
    case FbxNodeAttribute::eUnknown: return "unidentified";
    case FbxNodeAttribute::eNull: return "null";
    case FbxNodeAttribute::eMarker: return "marker";
    case FbxNodeAttribute::eSkeleton: return "skeleton";
    case FbxNodeAttribute::eMesh: return "mesh";
    case FbxNodeAttribute::eNurbs: return "nurbs";
    case FbxNodeAttribute::ePatch: return "patch";
    case FbxNodeAttribute::eCamera: return "camera";
    case FbxNodeAttribute::eCameraStereo: return "stereo";
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case FbxNodeAttribute::eLight: return "light";
    case FbxNodeAttribute::eOpticalReference: return "optical reference";
    case FbxNodeAttribute::eOpticalMarker: return "marker";
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary: return "boundary";
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case FbxNodeAttribute::eShape: return "shape";
    case FbxNodeAttribute::eLODGroup: return "lodgroup";
    case FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
}

void FbxSdkHelp::PrintAttribute(FbxNodeAttribute* pAttribute)
{
    if (!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();

    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void FbxSdkHelp::PrintNode(FbxNode* pNode)
{
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
           nodeName,
           translation[0], translation[1], translation[2],
           rotation[0], rotation[1], rotation[2],
           scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // Print the node's attributes.
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}
