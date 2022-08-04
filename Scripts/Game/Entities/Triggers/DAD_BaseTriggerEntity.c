[EntityEditorProps(category: "GameScripted/Triggers", description: "")]
class DAD_BaseTriggerEntityClass: SCR_BaseTriggerEntityClass
{
};

class DAD_BaseTriggerEntity: SCR_BaseTriggerEntity
{
	[Attribute(defvalue: "1", UIWidgets.EditBox, "Time in seconds to wait before checks will be allowed.", params: "0.1 inf")]
	protected float m_fInitialDelay;

	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (GetGame().GetWorld().GetWorldTime() < m_fInitialDelay) return false;
		return DefaultEntityFilterForQuery(ent);
	}

}
