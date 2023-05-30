class DAD_FollowAction : ScriptedUserAction {

	// TODO: It might be bad keeping a waypoint around for every Character that is FollowAction'ed
	SCR_EntityWaypoint m_FollowWaypoint = null;

	const ResourceName m_WaypointType = "{A0509D3C4DD4475E}Prefabs/AI/Waypoints/AIWaypoint_Follow.et";
	
	AIControlComponent m_AIControlComp = null;


	bool m_IsFollowing = false;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RplComponent rplC = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		rplC.InsertToReplication();
		if (!rplC.IsOwner()) return;

		if (!m_FollowWaypoint)
		{
			Resource wpRes = Resource.Load(m_WaypointType);
			m_FollowWaypoint = SCR_EntityWaypoint.Cast(SpawnHelpers.SpawnEntity(wpRes, pUserEntity.GetOrigin()));
		}

		AIControlComponent controlComp = AIControlComponent.Cast(pOwnerEntity.FindComponent(AIControlComponent));
		AIGroup ai = controlComp.GetControlAIAgent().GetParentGroup();

		CheckIsFollowing();
		if (m_IsFollowing)
		{
			ai.RemoveWaypointAt(0);
			return;
		}

		m_FollowWaypoint.SetEntity(pUserEntity);
		//m_FollowWaypoint.SetPriority(true);

		ai.AddWaypointAt(m_FollowWaypoint, 0);
	}

	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{	
		string readyText;
		
		if (CheckIsFollowing())
			outName = "Stop from Following";
		else
			outName = "Ask to Follow";

		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		SCR_ChimeraCharacter owner = SCR_ChimeraCharacter.Cast(GetOwner());
		SCR_ChimeraCharacter player = SCR_ChimeraCharacter.Cast(user);
		if (!owner.GetFaction().IsFactionFriendly(player.GetFaction())) return false;
		
		//AIControlComponent aiComp = AIControlComponent.Cast(owner.FindComponent(AIControlComponent));
		//if (!aiComp.IsAIActivated()) return false; 

		return true;
	}

	override bool CanBroadcastScript()
	{
		return true;
	}

	AIGroup GetAI()
	{
		AIControlComponent controlComp = AIControlComponent.Cast(GetOwner().FindComponent(AIControlComponent));
		AIGroup ai = controlComp.GetControlAIAgent().GetParentGroup();
		
		if (!ai)
		{
			Print("DAD_FollowAction: Could not find AI Group", LogLevel.ERROR);
			return null;
		}
		return ai;
	}

	bool CheckIsFollowing()
	{
		RplComponent rplC = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		rplC.InsertToReplication();

		if (!rplC.IsOwner()) return m_IsFollowing;

		AIGroup ai = GetAI();
		AIWaypoint wp = ai.GetCurrentWaypoint();

		if (!wp)
		{
			m_IsFollowing = false;
		 	return m_IsFollowing;
		}
		
		auto prefab = wp.GetPrefabData();
		m_IsFollowing = prefab && prefab.GetPrefabName() == m_WaypointType;
		return m_IsFollowing;
	}
}
