
/******************************************************************************/

#ifndef _S_TIROMAPOBJECT_H_
#define _S_TIROMAPOBJECT_H_

/******************************************************************************/

#include "MapServerPreqs.h"
#include "SMapObject.h"
#include "SSceneObjectFactory.h"
#include "STiroSceneObject.h"

/******************************************************************************/

namespace MG
{
    /******************************************************************************/
    //  ���ִ��ͼ������
    /******************************************************************************/
    class STiroMapObject : public SMapObject
    {
    public:

        STiroMapObject();
        virtual ~STiroMapObject();

        //-----------------------------------------------------------------------------------
        // Main
        //-----------------------------------------------------------------------------------

        virtual Bool            initialize(){return 0;}
        virtual Bool            unInitialize(){return 0;}
        virtual void            clear(){return ;}
        virtual void            update(Flt delta){return ;}

        //-----------------------------------------------------------------------------------
        // Base Data
        //-----------------------------------------------------------------------------------

        

        //-----------------------------------------------------------------------------------
        // Scene
        //-----------------------------------------------------------------------------------

		//�����б�
		void		createScene(STiroSceneObjectPtr& ptr, InstanceUIdType instanceUid);
		Bool		getScene(STiroSceneObjectPtr& ptr, InstanceUIdType instanceUid);
		void		destroyScene(InstanceUIdType instanceUid);
		void		destroyAllScene();

    private:

		// �����ִ帱����ͼ������
		STiroSceneObjectFactory		mSTiroSceneObjectFactory;

		//�����ִ帱����ͼ���б�
		std::map<InstanceUIdType,STiroSceneObjectPtr> mSTiroSceneObjecList;


		RWLocker    mSTiroSceneObjectListCs;



    };


}

#endif	