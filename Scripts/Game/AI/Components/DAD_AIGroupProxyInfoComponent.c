[ComponentEditorProps(category: "GameScripted/AI", description: "Component to expose relevant AI Group data to proxies")]
class DAD_AIGroupProxyInfoComponentClass: SCR_AIInfoBaseComponentClass
{
};

enum DAD_E3GroupActionState: EGroupState
{
	LAST,
};

//------------------------------------------------------------------------------------------------
class DAD_AIGroupProxyInfoComponent : SCR_AIInfoBaseComponent
{
	private EGroupState m_EGroupState;

	protected ref ScriptInvoker Event_OnActionStateChanged = new ScriptInvoker;

	//------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);

		if (!Replication.IsServer())
			return;

		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(owner);

		AIControlComponent controlComp = AIControlComponent.Cast(owner.FindComponent(AIControlComponent));
		//m_AI = controlComp.GetControlAIAgent().GetParentGroup();
	}

	void UpdateInfo(AIGroup ai)
	{
	}

	//------------------------------------------------------------------------------------------------
	void SetGroupActionState(EGroupState currActionState) // This property just contains info, it doesn't change group behaviour!
	{
		#ifdef AI_DEBUG
		AddDebugMessage(string.Format("SetGroupActionState: %1", typename.EnumToString(EGroupState, currActionState)), msgType:EAIDebugMsgType.INFO);
		#endif

		if (currActionState == m_EGroupState)
			return;

		if (!Replication.IsServer())
			return;

		RplSetGroupActionState(currActionState);
		Rpc(RplSetGroupActionState, currActionState);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RplSetGroupActionState(EGroupState currActionState)
	{
		m_EGroupState = currActionState;
		Event_OnActionStateChanged.Invoke(currActionState);
	}

	//------------------------------------------------------------------------------------------------
	EGroupState GetGroupActionState()
	{
		return m_EGroupState;
	}

	//------------------------------------------------------------------------------------------------
	ScriptInvoker GetOnActionStateChanged()
	{
		return Event_OnActionStateChanged;
	}

	//======================================== RPL ========================================\\
	override bool RplSave(ScriptBitWriter writer)
    {
        writer.WriteInt(m_EGroupState);

        return true;
    }

    override bool RplLoad(ScriptBitReader reader)
    {
		EGroupState groupActionState;

        reader.ReadInt(groupActionState);

		RplSetGroupActionState(groupActionState);

        return true;
    }
};
