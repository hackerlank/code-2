/******************************************************************************/
#include "stdafx.h"
#include "ClanDBSystem.h"
#include "MySqlDriver.h"
#include "DBQueryResult.h"
#include "DBDriver.h"
#include "CommonGameDBOp.h"
#include "ClanQuestObject.h"
#include "QuestList.h"
#include "PlayerItem.h"

/******************************************************************************/




/******************************************************************************/
namespace MG
{
    Bool ClanDBSystem::checkOrCreateClanDataBaseTable( DBDriver* driver )
    {
        static Colume clandatabaseColumn[] = 
        {
			{AccountId,			"BIGINT",			"0"},                                 //�ʺ�Id
            {Clan_Id,			"INT",				"0"},                                 //����Id
			//{Genreal_Id_List,	"VARCHAR(1000)",	"0"},                                 //�佫Id�б�
            {MainGenereal_id,	"BIGINT",			"0"},                                 //���佫
            {Region_Id,			"INT",				"0"},                                 //regionId
            {Region_Type,		"TINYINT",			"0"},                                 //regionType
            {Status,			"TINYINT",			"0"},                                 //״̬
            {Ancestry,			"TINYINT",			"0"},                                 //Ѫͳ
            {Homeland_Name,		"VARCHAR(1000)",	"0"},                                 //����
            {Name,				"VARCHAR(1000)",	"0"},                                 //��
            {Blem,				"TINYINT",			"0"},                                 //���
            {Blem_color,		"TINYINT",			"0"},                                 //��ɫ
            {BackGroud,			"TINYINT",			"0"},                                 //����
            {BackGroud_color,	"TINYINT",			"0"},                                 //������ɫ
            {Login_num,			"INT",				"0"},                                 //��¼����
            {Clanbag_size,		"INT",				"0"},                                 //���屳���Ĵ�С
			{Bag_item,			"VARCHAR(10000)",	"0"},                                 //���屳����Ʒ
            {Quest_doing,		"VARCHAR(1000)",	"0"},                                 //��������������¼
			{Quest_finished,	"VARCHAR(1000)",	"0"},                                 //��������������¼
			{money,				"INT",				"0"}									//������Ϸ��
            /*{"clandb_create_time",      "DATETIME",           "0"},
            {"clandb_online_time",      "DATETIME",             "0"},
            {"clandb_delete_time",      "DATETIME",             "0"},
            {"clandb_login_time",       "DATETIME",             "0"},
            {"clandb_logout_time",      "DATETIME",             "0"}*/
        };

		Int row = sizeof(clandatabaseColumn)/sizeof(Colume);
		DYNAMIC_ASSERT( row == ClanDBRow_Max);
		DBQueryResultEx resEx = driver->query("CREATE TABLE IF NOT EXISTS " CLANDATABASE_TABLE_NAME 
											  " ( " AccountId " BIGINT NOT NULL, PRIMARY KEY (" AccountId ") )");
        DBQueryResult* res1 = *resEx;
        if (!res1 || res1->getResultType() == DBQuery_Result_INVALID)
        {
            return false;
        }

        Bool  res2 = driver->addColume( CLANDATABASE_TABLE_NAME, clandatabaseColumn, sizeof(clandatabaseColumn)/sizeof(Colume) );
        if ( !res2 )
        {
            return false;
        }

        ////��ҵ�accountid,һ����ҿ�ӵ�ж�����壬����Ψһ
        //if ( !( *( driver.query("CREATE INDEX accountid ON clandatabase_info (clandb_account_id)") ) ) )
        //{
        //    return false;
        //}

        ////�佫��id��Ψһ�ģ��佫������ӵ�У�������һ����ֻ�ܳ���һ��
        //if ( !( *( driver.query("CREATE INDEX maingenrealid ON clandatabase_info (clandb_maingenreal_id)") ) ) )
        //{
        //    return false;
        //}

        return true;
    }
    //------------------------------------------------------------------------------------
	Bool ClanDBSystem::getClanDataBase( DBDriver* driver, 
										AccountIdType accountId, 
										std::vector<ClanData>				*	clanDataList,
										std::vector<dbDoingClanQuestObject>	*	doingQuestData,
										std::vector<FinishedClanQuestObject>*	finishedQuestData, 
										std::vector<ItemRecord>				*	recordList )
    {
        ClanData data;

        Char8 sql[128] = {0};
        MGStrOp::sprintf(sql,128,"SELECT * FROM " CLANDATABASE_TABLE_NAME 
								 " WHERE " AccountId " = %I64d"  ,
								 accountId);

        DBQueryResultEx resEx = driver->query(sql);
        DBQueryResult* res = *resEx;
        if ( res  &&  res->getResultType() != DBQuery_Result_INVALID  &&  res->getRowCount() > 0 )
        {
            Str16 wName;
			Str16 questDoingRecordString;
			Str16 questFinishedRecordString;
            Str   tempStr;
            
			//clanItemDB
			Str8 dbClanItemRecordString;
            I32 nRow = res->getRowCount();
			//����һ�����ֻ��һ������,��ӦΨһ�������¼,�ж��������Ҫ�޸�
			if ( nRow > 1 )
			{
				DYNAMIC_ASSERT(0);
			}

            for ( I32 i=0; i<nRow; i++ )
            {
                memset(&data, 0, sizeof(data));
                U32 nCol = 0;

				data.accountId          = res->getFieldData( i, ClanDBRow_AccountId )->ulonglongData;
                data.clanId             = res->getFieldData( i, ClanDBRow_Clan_Id )->uintData;
                data.mainGenrealId      = res->getFieldData( i, ClanDBRow_MainGenereal_id )->ulonglongData;
                data.regionID           = res->getFieldData( i, ClanDBRow_Region_Id )->ulonglongData;
                data.regionType         = (REGION_OBJ_TYPE)res->getFieldData( i, ClanDBRow_Region_Type )->ubyteData;
                data.status             = (CLAN_STATUS_TYPE)res->getFieldData( i, ClanDBRow_Status )->ubyteData;
                data.ConstellationId    = res->getFieldData( i, ClanDBRow_Ancestry)->ubyteData;
             
                res->getFieldData(i, ClanDBRow_Homeland_Name, tempStr);
                MGStrOp::toString(tempStr.c_str(), wName);
                data.clanHomeLandName   = wName;
                res->getFieldData(i, ClanDBRow_Name, tempStr);
                MGStrOp::toString(tempStr.c_str(), wName);
                data.clanName           = wName;
                data.armyemblem         = res->getFieldData( i, ClanDBRow_Blem )->ubyteData;
                data.armyemblemcolor    = res->getFieldData( i, ClanDBRow_Blem_color )->ubyteData;
                data.bg                 = res->getFieldData( i, ClanDBRow_BackGroud )->ubyteData;
                data.bgcolor            = res->getFieldData( i, ClanDBRow_BackGroud_color )->ubyteData;
                data.loginnum           = res->getFieldData( i, ClanDBRow_Login_num )->uintData;
                data.clanbagSize        = res->getFieldData( i, ClanDBRow_Clanbag_size )->uintData;
				res->getFieldData(i, ClanDBRow_Quest_doing, tempStr);
                MGStrOp::toString(tempStr.c_str(), questDoingRecordString);
				res->getFieldData(i, ClanDBRow_Quest_finished, tempStr);
                MGStrOp::toString(tempStr.c_str(), questFinishedRecordString);
                
				res->getFieldData(i, ClanDBRow_money, dbClanItemRecordString);
				//data.clanGameMoney = res->getFieldData(i, nCol++)->uintData;
                /*res->getFieldData(i, nCol++, tempStr);
                sscanf_s(tempStr.c_str(),"%d-%d-%d %d:%d:%d",&nY,&nM,&nD,&nH,&nMi,&nS);
                data.createtime  = MGTimeOp::createtimestamp(nY,nM,nD,nH,nMi,nS);
                res->getFieldData(i, nCol++, tempStr);
                sscanf_s(tempStr.c_str(),"%d-%d-%d %d:%d:%d",&nY,&nM,&nD,&nH,&nMi,&nS);
                data.onlinetime  = MGTimeOp::createtimestamp(nY,nM,nD,nH,nMi,nS);
                res->getFieldData(i, nCol++, tempStr);
                sscanf_s(tempStr.c_str(),"%d-%d-%d %d:%d:%d",&nY,&nM,&nD,&nH,&nMi,&nS);
                data.deletetime  = MGTimeOp::createtimestamp(nY,nM,nD,nH,nMi,nS);
                res->getFieldData(i, nCol++, tempStr);
                sscanf_s(tempStr.c_str(),"%d-%d-%d %d:%d:%d",&nY,&nM,&nD,&nH,&nMi,&nS);
                data.logintime  = MGTimeOp::createtimestamp(nY,nM,nD,nH,nMi,nS);
                res->getFieldData(i, nCol++, tempStr);
                sscanf_s(tempStr.c_str(),"%d-%d-%d %d:%d:%d",&nY,&nM,&nD,&nH,&nMi,&nS);
                data.logouttime  = MGTimeOp::createtimestamp(nY,nM,nD,nH,nMi,nS);*/
                //nameList->push_back(data);

				//����������Ϣ
                loadQuest(questDoingRecordString,questFinishedRecordString,doingQuestData,finishedQuestData);
				
				//�������屳����Ʒ��Ϣ
                loadClanItem(dbClanItemRecordString,recordList);
				
            }
            return true;
        }

        return false;
    }
    //------------------------------------------------------------------------------------
    Bool ClanDBSystem::insertClanDataBase(DBDriver* driver, const ClanData* clanData)
    {
		Str8 templandname, tempname;
		MGStrOp::toString( clanData->clanHomeLandName.c_str(), templandname );
		MGStrOp::toString( clanData->clanName.c_str(), tempname );

        //����
        Char8 sql[1024] = {0};
        MGStrOp::sprintf(
                            sql,
                            1024, 
							"insert into clandatabase_info ("
							AccountId			", "
							Clan_Id				", "
							MainGenereal_id		", "
							Region_Id			", "
							Region_Type			", "
							
							Status				", "
							Ancestry			", "
							Homeland_Name		", "
							Name				", "
							Blem				", "
							Blem_color			", "

							BackGroud			", "
							BackGroud_color		", "
							Login_num			", "
							Clanbag_size		", "
							Quest_doing			", "

							Quest_finished		", "
							Bag_item			", "
							money")"
							"values(%I64d, %I64d, '', %I64i, %d, %d,\
									 %d, %d, '', '', %d, %d, \
									%d, %d, %d, %d, '',\
									 '', '', %d)" ,
							clanData->accountId, clanData->clanId, clanData->mainGenrealId,clanData->regionID, clanData->regionType, 
							clanData->status,clanData->ConstellationId, /*templandname.c_str(), tempname.c_str(),  */clanData->armyemblem, clanData->armyemblemcolor,
							clanData->bg, clanData->bgcolor, clanData->loginnum, clanData->clanbagSize, 
							0
                         );
                    
        Bool result = false; 
        driver->queryLock();
        {

            DBQueryResultEx resEx = driver->query( sql, false );
            DBQueryResult* dbRes = *resEx;
            if ( dbRes  &&  dbRes->getResultType() != DBQuery_Result_INVALID )
            {  
                result = true;
            }
        }
        driver->queryUnLock();

        return false;
    }

	//------------------------------------------------------------------------------------
    Bool ClanDBSystem::deleteClanDataBase(DBDriver* driver, AccountIdType accountId, ClanIdType clanId)
    {

		Char8 sql[128] = {0};
		MGStrOp::sprintf(
			                sql,
							128,
							"DELETE from " CLANDATABASE_TABLE_NAME " WHERE " AccountId  " = %I64d AND " Clan_Id" = %I64d",
                            accountId, clanId
			            );
	    DBQueryResultEx resEx = driver->query( sql, false );
		DBQueryResult* dbRes = *resEx;
		if ( dbRes  &&  dbRes->getResultType() != DBQuery_Result_INVALID )
		{  
			return true;
		}

        return false;
    }


	//------------------------------------------------------------------------------------
	void ClanDBSystem::loadQuest(const Str16& dbQuestDoingRecordString,
								 const Str16& dbQuestFinishedRecordString,
								 std::vector<dbDoingClanQuestObject>* doingQuestData,
								 std::vector<FinishedClanQuestObject>* finishedQuestData)
	{

		//doingrecorddoing��ʽ
		//questID.completestate.begintime.endtime.value1.value2.value3.value4,
		//uestID.completestate.begintime.endtime.value1.value2.value3.value4,...
		//

		//doing quest data
		std::vector<std::vector<Str16>> questDoingRecords;
		std::vector<Str16>              tempDoingRecords;  
		std::vector<Str16>              tempFinishedRecords;
		MGStrOp::split<Str16>(dbQuestDoingRecordString,DB_RECORDS_SEPARATOR,tempDoingRecords);
		if (tempDoingRecords.size() >0 && 
			dbQuestDoingRecordString.length() > 0 && dbQuestDoingRecordString[0] != L'\0')
		{   
			//��Ӧ��¼���ֶ�
			I32 questId =0;
			I32 completeState = 0;
			U64 beginTime = 0;
			U64 endTime = 0;
			I32 value1 = 0;
			I32 value2 = 0;
			I32 value3 = 0;
			I32 value4 = 0;
			dbDoingClanQuestObject questObj;

			std::vector<Str16>::iterator front = tempDoingRecords.begin();
			for (; front != tempDoingRecords.end();front++)
			{
				std::vector<Str16> FieldsInRecord;
				MGStrOp::split<Str16>((*front),DB_FIELDS_SEPARATOR,FieldsInRecord);
				MGStrOp::toI32(FieldsInRecord[0].c_str(),questId);
				questObj.questInfoId = questId;
				MGStrOp::toI32(FieldsInRecord[1].c_str(),completeState);
				questObj.mRecord.state = (QUEST_COMPLETE_STATE)completeState;
				MGStrOp::toU64(FieldsInRecord[2].c_str(),beginTime);
				questObj.mRecord.beginTime = beginTime;
				MGStrOp::toU64(FieldsInRecord[3].c_str(),endTime);
				questObj.mRecord.endTime = endTime;
				MGStrOp::toI32(FieldsInRecord[4].c_str(),value1);
				questObj.mRecord.value[0] = value1;
				MGStrOp::toI32(FieldsInRecord[5].c_str(),value2);
				questObj.mRecord.value[1] = value2;
				MGStrOp::toI32(FieldsInRecord[6].c_str(),value3);
				questObj.mRecord.value[2] = value3;
				MGStrOp::toI32(FieldsInRecord[7].c_str(),value4);
				questObj.mRecord.value[3] = value4; 

				//
				doingQuestData->push_back(questObj);

			}

		}

		//finished quest data
		MGStrOp::split<Str16>(dbQuestFinishedRecordString,DB_RECORDS_SEPARATOR,tempFinishedRecords);
		if (tempFinishedRecords.size() >0 
			&& dbQuestFinishedRecordString.length() >0
			&& dbQuestFinishedRecordString[0] != L'\0')
		{
			//��Ӧ��¼���ֶ�
			I32 questId = 0;
			U32 endTime = 0;
			FinishedClanQuestObject questObj;
			std::vector<Str16>::iterator front = tempFinishedRecords.begin();
			for (; front != tempFinishedRecords.end();front++)
			{
				std::vector<Str16> FieldsInRecord;
				MGStrOp::split<Str16>((*front),DB_FIELDS_SEPARATOR,FieldsInRecord);
				MGStrOp::toI32(FieldsInRecord[0].c_str(),questId);
				questObj.questInfoId = questId;
				MGStrOp::toU32(FieldsInRecord[1].c_str(),endTime);
				questObj.endTime = endTime;

				//
				finishedQuestData->push_back(questObj);

			}
		}

	}


	//------------------------------------------------------------------------------------
	void ClanDBSystem::loadClanItem( const Str8 dbClanItemRecordString,std::vector<ItemRecord>* recordList )
	{
        //
        std::vector<Str8> tempClanItemRecords;
        MGStrOp::split<Str8>(dbClanItemRecordString,DB_RECORDS_SEPARATOR,tempClanItemRecords);
		if (tempClanItemRecords.size() >0 && 
			dbClanItemRecordString.length() > 0 && dbClanItemRecordString[0] != L'\0')
		{
             //��Ӧ���ݿ��е��ֶ�
			IdType              itemDBID            = 0;
			U32                 itemTempID          = 0;
			/*ITEM_BELONG_TYPE*/I32    itemBelongType      = 0;
			IdType              itemBelongID        = 0;  //��Ʒ�󶨽�ɫ�������
			/*ITEM_SITE_TYPE*/I32      belongPos           = 0;      //��Ʒ��ǰ���ڸ�λ
			/*ITEM_BIND_TYPE*/I32      bindType            = 0;
			U32                 slotIndex               = 0;
			Char16              fatherName[MAX_CHARACTER_NAME_LEN] = {0};
			U32                 curNum              = 0;
			U32                 curDurable          = 0;
			U32                 curRepairNum        = 0;
			U32                 curStrengthenLv     = 0;
			U32                 randAppendID        = 0;
			U32                 strengthenAppendID  = 0;
			U64                 dueTime             = 0;
			U64                 bornTime            = 0;
			U64                 delTime             = 0;
			U64                 updateTime          = 0;
			//
			
			ItemRecord  itemRecordObject;
			std::vector<Str8>::iterator front = tempClanItemRecords.begin();
			std::vector<Str8>::iterator back = tempClanItemRecords.end();
			while(front != back)
			{
                 std::vector<Str8> FieldsInRecord;
				  MGStrOp::split<Str8>((*front),DB_FIELDS_SEPARATOR,FieldsInRecord);
				  MGStrOp::toU64(FieldsInRecord[0].c_str(),itemDBID);
				  itemRecordObject.itemDBID = itemDBID;
				  MGStrOp::toU32(FieldsInRecord[1].c_str(),itemTempID);
                  itemRecordObject.itemTempID = itemTempID;
				  MGStrOp::toI32(FieldsInRecord[2].c_str(),itemBelongType);
				  itemRecordObject.itemBelongType = (ITEM_BELONG_TYPE)itemBelongType;
				  MGStrOp::toU64(FieldsInRecord[3].c_str(),itemBelongID);
				  itemRecordObject.itemBelongID = itemBelongID;
				  MGStrOp::toI32(FieldsInRecord[4].c_str(),belongPos);
				  itemRecordObject.belongPos = (ITEM_SITE_TYPE)belongPos;
				  MGStrOp::toI32(FieldsInRecord[5].c_str(),bindType);
				  itemRecordObject.bindType = (ITEM_BIND_TYPE)bindType;
				  MGStrOp::toU32(FieldsInRecord[6].c_str(),slotIndex);
				  itemRecordObject.slotIndex = slotIndex;
				  //
				  Str16 tempName;
				  MGStrOp::toString(FieldsInRecord[7].c_str(),tempName);
				  memcpy(itemRecordObject.fatherName,tempName.c_str(), sizeof(MAX_CHARACTER_NAME_LEN));
				  MGStrOp::toU32(FieldsInRecord[8].c_str(),curNum);
				  itemRecordObject.curNum = curNum;
				  MGStrOp::toU32(FieldsInRecord[9].c_str(),curDurable);
				  itemRecordObject.curDurable = curDurable;
				  MGStrOp::toU32(FieldsInRecord[10].c_str(),curRepairNum);
				  itemRecordObject.curRepairNum = curRepairNum;
				  MGStrOp::toU32(FieldsInRecord[11].c_str(),curStrengthenLv);
				  itemRecordObject.curStrengthenLv = curStrengthenLv;
				  MGStrOp::toU32(FieldsInRecord[12].c_str(),randAppendID);
				  itemRecordObject.randAppendID = randAppendID;
				  MGStrOp::toU32(FieldsInRecord[13].c_str(),strengthenAppendID);
				  itemRecordObject.strengthenAppendID = strengthenAppendID;
				  MGStrOp::toU64(FieldsInRecord[14].c_str(),dueTime);
				  itemRecordObject.delTime = dueTime;
				  MGStrOp::toU64(FieldsInRecord[15].c_str(),bornTime);
				  itemRecordObject.bornTime = bornTime;
				  MGStrOp::toU64(FieldsInRecord[16].c_str(),delTime);
				  itemRecordObject.delTime = delTime;
				  MGStrOp::toU64(FieldsInRecord[17].c_str(),updateTime);
				  itemRecordObject.updateTime = updateTime;

				  recordList->push_back(itemRecordObject);
                  front++;
			}

		}

	}
	//------------------------------------------------------------------------------------
	MG::Bool ClanDBSystem::updateClanDataBase( DBDriver* driver, const ClanData* clanData )
	{
		Str8 templandname, tempname;
		MGStrOp::toString( clanData->clanHomeLandName.c_str(), templandname );
		MGStrOp::toString( clanData->clanName.c_str(), tempname );

		//����
		Char8 sql[1024] = {0};
		MGStrOp::sprintf(
			sql,
			1024, 
			"update clandatabase_info set "
			AccountId			" =  %I64d, "
			Clan_Id				" = %d, "
			MainGenereal_id		" = %d, "
			Region_Id			" = %d, "
			Region_Type			" = %d, "

			Status				" = %d, "
			Ancestry			" = %d, "
			Homeland_Name		" = '%s', "
			Name				" = '%s', "
			Blem				" = %d, "
			Blem_color			" = %d, "

			BackGroud			" = %d, "
			BackGroud_color		" = %d, "
			Login_num			" = %d, "
			Clanbag_size		" = %d, "
			Quest_doing			" = '', "

			Quest_finished		" = '', "
			Bag_item			" = '', "
			money				" =	= %d" ,
			clanData->accountId, clanData->clanId, clanData->mainGenrealId,clanData->regionID, clanData->regionType, 
			clanData->status,clanData->ConstellationId, templandname.c_str(), tempname.c_str(),  clanData->armyemblem, clanData->armyemblemcolor,
			clanData->bg, clanData->bgcolor, clanData->loginnum, clanData->clanbagSize, 
			0
			);

		Bool result = false; 
		driver->queryLock();
		{

			DBQueryResultEx resEx = driver->query( sql, false );
			DBQueryResult* dbRes = *resEx;
			if ( dbRes  &&  dbRes->getResultType() != DBQuery_Result_INVALID )
			{  
				driver->queryUnLock();
				return true;
			}
		}
		driver->queryUnLock();

		return false;
	}

}