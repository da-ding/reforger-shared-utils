[EntityEditorProps(category: "GameScripted/Triggers", description: "")]
class DAD_LootboxRadiusEntityClass: DAD_SpawnRadiusEntityClass
{
};

class DAD_LootboxRadiusEntity: DAD_SpawnRadiusEntity
{
	[Attribute(uiwidget: UIWidgets.ResourceNamePicker, desc: "The 'container' to spawn in the same place as the item", params: "et", category: "Spawn Options")]

	ResourceName m_eLootbox;

	override void OnActivate(IEntity ent)
	{

		RplComponent rplC = RplComponent.Cast(this.FindComponent(RplComponent));
		if ((rplC && !rplC.IsOwner()) || !Replication.IsServer())
			return;

		SpawnHelpers.SpawnLootboxPoolInRadius(m_aEntitySpawnSlots, m_fSpawnCount, this.GetOrigin(), this.GetSphereRadius(), m_eLootbox, m_bRandomChoose);
	}


};
