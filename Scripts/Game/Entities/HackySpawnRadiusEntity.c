[EntityEditorProps(category: "GameScripted/Triggers", description: "")]
class DAD_SpawnRadiusEntityClass: ScriptedGameTriggerEntityClass
{
};

class DAD_SpawnRadiusEntity: ScriptedGameTriggerEntity
{
	static ref RandomGenerator RNG = new RandomGenerator();

	protected ref array<IEntity> m_Entities;
	array<IEntity> GetEntities() return m_Entities; 

	[Attribute(uiwidget: UIWidgets.ResourceAssignArray, desc: "List of Entities to Spawn from", params: "et", category: "Spawn Options")]
	ref array<ResourceName> m_aEntitySpawnSlots;
	
	[Attribute(defvalue: "1", UIWidgets.EditBox, desc: "Number of objects to spawn out of spawn pool", category: "Spawn Options")]
	protected int m_fSpawnCount;
	
	[Attribute(defvalue: "1", UIWidgets.EditBox, desc: "When enabled, randomly choose entities from the spawn pool, instead of looping over list.", category: "Spawn Options")]
	protected bool m_bRandomChoose;

	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		return ent == this; // Trigger once per trigger-cycle
	}
	
	override void OnActivate(IEntity ent)
	{
		super.OnActivate(ent);
		m_Entities = SpawnHelpers.SpawnPoolInRadius(m_aEntitySpawnSlots, m_fSpawnCount, this.GetOrigin(), this.GetSphereRadius(), m_bRandomChoose);
	}
	

};
