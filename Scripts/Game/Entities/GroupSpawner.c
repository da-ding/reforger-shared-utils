[EntityEditorProps(category: "GameScripted/GameMode", description: "Spawn point entity", visible: false)]
class DAD_GroupSpawnPointClass : SCR_SpawnPointClass
{
};
//------------------------------------------------------------------------------------------------
//! Spawn point entity defines positions on which players can possibly spawn.
class DAD_GroupSpawnPoint : SCR_SpawnPoint
{
	protected SCR_AIGroup m_Group;

	void SetGroup(SCR_AIGroup group)
		m_Group = group;

	SCR_AIGroup GetGroup()
		return m_Group;
	
	override void EOnPlayerSpawn(IEntity entity)
	{
		super.EOnPlayerSpawn(entity);
		int last = m_Group.m_aSceneGroupUnitInstances.Count() - 1;
		IEntity ai = m_Group.m_aSceneGroupUnitInstances.Get(last);
		m_Group.m_aSceneGroupUnitInstances.Remove(last);
		m_Group.RemoveAIEntityFromGroup(ai);
		SCR_EntityHelper.DeleteEntityAndChildren(ai);

		// TODO: How do I get the player ID out of the IEntity?
		// Oh yeah, also, adding players to the group means that players might get yeeted
		// if someone spawns to a group that already has players!
		// m_Group.AddPlayer(entity);
	}

	protected void UpdatePosition()
	{
		if (!m_Group) return;
		SetOrigin(m_Group.GetOrigin());
		Update();
	}

	override void EOnInit(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(UpdatePosition, 2, true);
	}

};
