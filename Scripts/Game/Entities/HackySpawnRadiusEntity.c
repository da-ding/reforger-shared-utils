[EntityEditorProps(category: "GameScripted/Triggers", description: "")]
class DAD_SpawnRadiusEntityClass: ScriptedGameTriggerEntityClass
{
};

class DAD_SpawnRadiusEntity: ScriptedGameTriggerEntity
{
	static ref RandomGenerator RNG = new RandomGenerator();

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
		SpawnHelpers.SpawnPoolInRadius(m_aEntitySpawnSlots, m_fSpawnCount, this.GetOrigin(), this.GetSphereRadius(), m_bRandomChoose);
		ent.Update();
		
		//Resource res = Resource.Load("{7A4A0A8283210784}Prefabs/Compositions/Misc/SubCompositions/TableMilitary_Cluttered_US_01.et");
		Resource res = Resource.Load("{53506DCB398C7EBE}Prefabs/Props/Furniture/Table_01.et");
		/* IEntity entity = SpawnHelpers.SpawnEntity(res, this.GetOrigin()); */
		IEntity entity = SpawnHelpers.SpawnRandomInRadius(res, this.GetOrigin(), this.GetSphereRadius());
		entity.Update();
		ent.Update();
	}
	

};
