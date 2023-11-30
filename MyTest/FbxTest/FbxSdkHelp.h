#pragma once

#include <fbxsdk.h>

class FbxSdkHelp
{
public:
    FbxSdkHelp();
    ~FbxSdkHelp();
    /// @brief 我的第一个程序
	void RunMyFirstProgrammer();
    /// @brief 插入节点测试
    void RunAddFbxNode();
    /// @brief 插入节点属性测试
    void RunFbxProperty();
    /// @brief 创建一个Mesh
    void RunCreateMeshes();
    
public:
    /// @brief 导出场景
    static  void ExportScene(FbxScene* pFbxScene, const char* lFilename = "file.fbx");

private:
    /// @brief 创建跟结点的子节点
    FbxNode* CreateFbxNode(FbxScene* pFbxScene);
    void CreateOctahedron(FbxMesh* pMesh);
    /// @brief Print the required number of tabs.
	void PrintTabs();
    /// @brief Return a string-based representation based on the attribute type.
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
    /// @brief Print an attribute.
	void PrintAttribute(FbxNodeAttribute* pAttribute);
    /// @brief Print a node, its attributes, and all its children recursively.
	void PrintNode(FbxNode* pNode);

private:
	int numTabs = 0;
    FbxManager* m_pSdkManager;
    FbxScene* m_pMyScene;
};
