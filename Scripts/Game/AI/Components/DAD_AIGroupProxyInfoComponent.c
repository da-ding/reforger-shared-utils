[ComponentEditorProps(category: "GameScripted/AI", description: "Component to expose relevant AI Group data to proxies")]
class DAD_AIGroupProxyInfoComponentClass: SCR_AIInfoBaseComponentClass
{
};

enum DAD_EGroupActionState
{
	NONE = 0,
	FOLLOWING,
	MOVING,
	LAST,
};

//------------------------------------------------------------------------------------------------
class DAD_AIGroupProxyInfoComponent : SCR_AIInfoBaseComponent
{
	private DAD_EGroupActionState m_DAD_EGroupActionState;
	
	protected ref ScriptInvoker Event_OnActionStateChanged = new ScriptInvoker;
	
	//------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);	
	}	
	
	//------------------------------------------------------------------------------------------------
	void SetGroupActionState(DAD_EGroupActionState currActionState) // This property just contains info, it doesn't change group behaviour!
	{
		#ifdef AI_DEBUG
		AddDebugMessage(string.Format("SetGroupActionState: %1", typename.EnumToString(DAD_EGroupActionState, currActionState)), msgType:EAIDebugMsgType.INFO);
		#endif
		
		if (currActionState == m_DAD_EGroupActionState)
			return;
		
		if (!Replication.IsServer())
			return;
		
		RplSetGroupActionState(currActionState);
		Rpc(RplSetGroupActionState, currActionState);	
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RplSetGroupActionState(DAD_EGroupActionState currActionState)
	{
		m_DAD_EGroupActionState = currActionState;
		Event_OnActionStateChanged.Invoke(currActionState);
	}
	
	//------------------------------------------------------------------------------------------------
	DAD_EGroupActionState GetGroupActionState()
	{
		return m_DAD_EGroupActionState; 
	}	
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GetOnActionStateChanged()
	{
		return Event_OnActionStateChanged;
	}	
	
	//======================================== RPL ========================================\\
	override bool RplSave(ScriptBitWriter writer)
    {	
        writer.WriteIntRange(m_DAD_EGroupActionState, 0, DAD_EGroupActionState.LAST-1);
		
        return true;
    }
     
    override bool RplLoad(ScriptBitReader reader)
    {
		DAD_EGroupActionState groupActionState;
		
        reader.ReadIntRange(groupActionState, 0, DAD_EGroupActionState.LAST-1);
		
		RplSetGroupActionState(groupActionState);
		
        return true;
    }
};
