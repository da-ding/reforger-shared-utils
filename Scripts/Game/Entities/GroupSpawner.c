[EntityEditorProps(category: "GameScripted/GameMode", description: "Spawn point entity", visible: false)]
class DAD_GroupSpawnPointClass : DAD_NotPlayerSpawnPointClass
{
};

class DAD_GroupSpawnPoint : DAD_NotPlayerSpawnPoint
{
	protected SCR_AIGroup m_Group;

	void SetGroup(SCR_AIGroup group)
	{
		if (m_Group) m_Group.GetOnEmpty().Remove(OnGroupEmpty);
		m_Group = group;
		m_TargetEntity = group;
		SetEntityID(group.GetName());
		
		m_Group.GetOnEmpty().Insert(OnGroupEmpty);
	}

	SCR_AIGroup GetGroup()
	{
		/* return SCR_AIGroup.Cast(m_TargetEntity); */
		return m_Group;
	}
	
	void OnGroupEmpty()
	{
		OnEntityDeleted(GetEntityID(), m_TargetEntity);
	}
	
	override void EOnPlayerSpawn(IEntity entity)
	{
		super.EOnPlayerSpawn(entity);
		
		if (!m_Group) return;

		//SCR_AIGroup m_Group = SCR_AIGroup.Cast(m_TargetEntity);

		// Delete a member from the group
		array<AIAgent> agents = new array<AIAgent>();
		m_Group.GetAgents(agents);
		int last = agents.Count() - 1;
		IEntity ai = agents.Get(last).GetControlledEntity();
		
		SCR_EntityHelper.DeleteEntityAndChildren(ai);
		m_Group.RemoveAIEntityFromGroup(ai);

		// TODO: How do I get the player ID out of the IEntity?
		// Oh yeah, also, adding players to the group means that players might get yeeted
		// if someone spawns to a group that already has players!
		// m_Group.AddPlayer(entity);
	}

	/* protected void UpdatePosition() */
	/* { */
	/* 	if (!m_Group) return; */
	/* 	SetOrigin(m_Group.GetOrigin()); */
	/* 	Update(); */
	/* } */

	/* override void EOnInit(IEntity owner) */
	/* { */
	/* 	GetGame().GetCallqueue().CallLater(UpdatePosition, 2, true); */
	/* } */

};
