/******************************************************************************/
#ifndef _SREGIONGRIDMANAGER_H_
#define _SREGIONGRIDMANAGER_H_
/******************************************************************************/

#include "MapServerPreqs.h"
/******************************************************************************/

/******************************************************************************/

namespace MG
{

	/******************************************************************************/
	// ��������һ�������������Щ���������й���
	/******************************************************************************/
	struct SSceneObjectInSceneGridsManager
	{

		SSceneGrid*					mLastSceneGrid;
		SSceneGrid*					mCurrSceneGrid;

		// ��ǰ��ע�������б�
		std::map<UInt, SSceneGrid*>	mInteractionGridList;
        Critical                    mInteractionGridListCs;

		SSceneObjectInSceneGridsManager()
			:mLastSceneGrid(NULL)
			,mCurrSceneGrid(NULL)
		{
		}
        
		~SSceneObjectInSceneGridsManager()
		{
			mLastSceneGrid = NULL;
			mCurrSceneGrid = NULL;
			mInteractionGridList.clear();
		}
	};


	/******************************************************************************/
	// SSceneGridManager
	/******************************************************************************/
	class SSceneGridManager
	{
	public:
		SSceneGridManager();
		virtual ~SSceneGridManager();


        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

		virtual Bool                            initialize( SSceneObject* sceneObj );
        virtual Bool                            unInitialize();
        virtual void                            clear();

	    virtual	void							update(Flt delta){ return ;}

        //-----------------------------------------------------------------------------------
        // Base Info
        //-----------------------------------------------------------------------------------

		UInt									getTemplateId();

        //-----------------------------------------------------------------------------------
        // Grid
        //-----------------------------------------------------------------------------------

    public:

		// ������
		SSceneGrid*							    getSceneGrid( Vec3 pos );

    protected:

		//-----------------------------------------------------------------------------------
		// Load Grid Map
		//-----------------------------------------------------------------------------------

		void									loadGridMap( SSceneObject* sceneObj );

        // �õ�һ����Χ�ڵ������б�
        void									getSceneGridsList( Vec3 pos, UInt width, std::map<UInt, SSceneGrid*>& gridsList );


        // 2�������б����
        // resultGridsList = gridsList1 - gridsList2 ( ��Χ����gridsList1 )
        void									subSceneGridsList( std::map<UInt, SSceneGrid*>& gridsList1, std::map<UInt, SSceneGrid*>& gridsList2, std::map<UInt, SSceneGrid*>& resultGridsList );


        // ������
        UInt									getSceneGridIndex( Vec3 pos );
        // ������
        VecU2									getSceneGridPos( Vec3 pos );
        // ������
        UInt									getSceneGridIndexForGridPos( VecU2 gridPos );

        // ������
        void									destroyAllGrid();

        //-----------------------------------------------------------------------------------
        // SCharacter
        //-----------------------------------------------------------------------------------

    public:

		// ���ӻ��ƶ�һ����ɫ������Ϣ�������б���
		void									addOrUpdateObjectInSceneGrid( SCharacter* character, Bool isFilterSelf = true );
        
		void									addOrUpdateObjectInSceneGrid( dropItemData* dropItem );
        // �Ƴ�һ����ɫ����
		void									removeObjectInSceneGrid( SCharacter* character, Bool isFilterSelf = true );

		// �Ƴ����н�ɫ����
		void									removeAllObjectInSceneGrid();

		
    protected:

		void									onEnterSceneGrid( SCharacter* character, SSceneGrid* currGrid, std::map<UInt, SSceneGrid*>& enterGridList, Bool isFilterSelf = true );
		void									onLeaveSceneGrid( SCharacter* character, std::map<UInt, SSceneGrid*>& leaveGridList, Bool isFilterSelf = true );

	protected:

		SRegionObjectBase*						mRegionObjectBase;
		SSceneGrid*							    mSceneGridList;
		U16										mGridWidthNum;
		U16										mGridHeightNum;
		U16										mGridWidth;
		U16										mGridHeight;
		Byte									mInteractionGridWidth;
		Byte									mInteractionGridHalfWidth;
		Byte									mNeedAddGridWidth;
		Byte									mNeedAddGridHalfWidth;
		UInt									mTemplateId;
		Vec3									mLeftBottomPos;
	};
}


#endif	