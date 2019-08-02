a-Opengl-Demo-with-glsl
=======================

a simple 3d opengl demo,with daynamc numbers of obeject and 3d btn and glsl,used a third-party:glew

ver="1.0"
mstrchs=#(\
#(1,"此生无白骨骼转换",""),\
#(2,"A","获取当前动画帧范围"),\
#(3,"设置目标骨骼",""),\
#(4,"刷新",""),\
#(5,"选择源骨骼",""),\
#(6,"设置源骨骼",""),\
#(7,"<--->","添加关联"),\
#(8,"<-X->","删除关联"),\
#(9," ↓ ","关联并顺序移到下一对"),\
#(10,"同步选择场景节点","设置关联时，同步选择场景中的物体"),\
#(11,"拷贝根节点位置",""),\
#(12,"拷贝根节点旋转",""),\
#(13,"开始帧：",""),\
#(14,"结束帧：",""),\
#(15,"拷贝姿势",""),\
#(16,"拷贝动画",""),\
#(17,"关于...",""),\
#(18,"用于此生无白的骨骼挂点旋转重置到Y轴Up",""),\
#(19,"1、虚拟点的父点必须叫Bip001，会自动断开虚拟点最后再重新链接",""),\
#(20,"2、选中root点再操作。",""),\
#(21,"有问题可以邮件联系wxm(dfghj77777@gmail.com)",""),\
#(22,"请选择一个物体","")\
)
mstren=#(\
#(1,"Boneanimation node reset for CSWB of Bojoy",""),\
#(2,"A","get the current timeline range"),\
#(3,"setTarget",""),\
#(4,"Refresh", ""),\
#(5,"selectSource", ""),\
#(6,"setSource", ""),\
#(7,"<--->", "add a linkage"),\
#(8,"<-X->", "delete a linkage"),\
#(9," ↓ ", "add a linkage, and move the cursor to the next"),\
#(10,"synchronized selection", ""),\
#(11,"copy rootnode position", ""),\
#(12,"copy rootnode rotation", ""),\
#(13,"start frame", ""),\
#(14,"end frame  ", ""),\
#(15,"copy posture", ""),\
#(16,"copy animation", ""),\
#(17,"about...",""),\
#(18,"用于此生无白的骨骼挂点旋转重置到Y轴Up",""),\
#(19,"1-",""),\
#(20,"2-",""),\
#(21,"Problems can e-mail contact wxm(dfghj77777@gmail.com)",""),\
#(22,"must select one object","")\
)

lang=sysinfo.getmaxlanguage()
mstr=mstren
  
try
(
	if lang[3]=="CHS"then mstr=mstrchs
		else mstr=mstren
)
catch(mstr=mstren)
	
Animtoolboxname=mstr[1][2]

try(destroydialog Anim_toolbox_rollout)catch()
try(destroydialog about_rollout)catch()

fn resetBonerot dstBone=
(
	if classof dstBone == Biped_Object then
	(
		biped.setTransform dstBone #rot eulerangles 45 45 0  true
	)
	else
	(

		ttf=dstBone.transform
		tempPos = ttf.pos
		ttf.rotation= eulerangles 0 0 0 
		ttf.pos = tempPos
		dstBone.transform=ttf

		format "%s \n" "dstBone.transform.rotation"
	)		
	
)

rollout about_rollout mstr[1][2]  width:256 height:260
(
	GroupBox grp mstr[1][2] pos:[8,8] width:240 height:240
	label lbl1 mstr[18][2] pos:[16,30] width:224 height:40
	label lbl2 mstr[19][2] pos:[16,70] width:224 height:90
	label lbl3 mstr[20][2] pos:[16,160] width:224 height:40
	label lbl4 mstr[21][2] pos:[16,200] width:224 height:30
)
_globalBip1 = nil
_globalParent1 = nil
_globalBip2 = nil
_globalParent2 = nil
_globalBip3 = nil
_globalParent3 = nil
rollout Anim_toolbox_rollout (Animtoolboxname+" V"+ver)
(
	button setdst mstr[3][2] pos:[20,10] width:80 height:30
	button refresh mstr[4][2] pos:[110,10] width:60 height:30
	dotNetControl tvdst "System.Windows.Forms.TreeView" pos:[20,50] width: 250 height:300
	button setsrc mstr[6][2] pos:[330,10] width:80 height:30
	dotNetControl tvsrc "TreeView" pos:[330,50] width: 200 height:300


	button showabout mstr[17][2] pos:[460,420] width:60 height:30

	
	fn addchild thenode thobj=
	(
		local thenode,newnode,thobj
		dataary=#(thobj.name,thobj,undefined)
		if thobj.name== "Bip001"then
		(
			_globalParent1 = thobj.parent
			_globalBip1 = thobj
			thobj.parent = nil
		)
		else if thobj.name== "eff_head"then
		(
			_globalParent2 = thobj.parent
			_globalBip2 = thobj
			thobj.parent = nil
			format "%s\n" "found head"
		)
		else if thobj.name== "eff_zero"then
		(
			_globalParent3 = thobj.parent
			_globalBip3 = thobj
			thobj.parent = nil
		)
		if thobj.name=="root" or thobj.name=="eff_body" or thobj.name=="eff_foot_l" or  thobj.name=="eff_foot_r" or thobj.name=="eff_hand_l" or thobj.name=="eff_hand_r" or thobj.name=="eff_head" or thobj.name=="eff_weapon_a" or thobj.name=="eff_zero"    then
		(
			resetBonerot thobj

			newnode=thenode.Nodes.add thobj.name
			newnode.tag=dotNetMXSValue dataary
		 
		format "%s\n" thobj.transform.rotation.x
		)
		for child in thobj.children do
		(	
			
			addchild thenode child		
			
		)
	)
	
	fn initTV tv obj=
	(	
		tv.BeginUpdate()
		tv.Nodes.clear()
		addchild tv obj
		_globalBip1.parent = _globalParent1
		_globalBip2.parent = _globalParent2
		_globalBip3.parent = _globalParent3
		--theRoot.BackColor=(dotNetClass "System.Drawing.Color").red
		--theRoot.BackColor=tv.BackColor

		tv.ExpandAll()
		tv.EndUpdate()

		tv.SelectedNode=tv.Nodes.Item[0]
	)
	
	fn getlinkary=
	(
		nodeary=(for i=0 to tvdst.nodes.count-1 collect tvdst.nodes.Item[i])
		for cnode in  nodeary do join nodeary (for i=0 to cnode.nodes.count-1 collect cnode.nodes.Item[i])
		
		linkary=#()
		for i= 1 to nodeary.count do 
		(
			dataary=nodeary[i].tag.value
			if dataary[3]!=undefined then
			(
				append linkary #(dataary[2],dataary[3])
			)
		)
		return linkary
	)
	
	on setsrc pressed do
	(
		format "preseed"
		if $selection.count==1 then
		(
			if isValidNode $selection[1] do initTV tvsrc $selection[1]
		)
		else
		(
			messagebox mstr[22][2]
		)
	)	
	
	on setdst pressed do
	(	
		if $selection.count==1 then
		(
			if isValidNode $selection[1] do initTV tvdst $selection[1]
					
		)
		else
		(
			messagebox mstr[22][2]
		)		
	)	
	
	fn selectNoderefObj EventArg=
	(
		if EventArg.Node==undefined then return false
		dstary=EventArg.Node.tag.value
		try(select dstary[2])catch(max select none)			
	)
	
	on tvsrc AfterSelect EventArg do
	(
		selectNoderefObj EventArg
	)
	
	on tvdst AfterSelect EventArg do
	(
		selectNoderefObj EventArg		
	)
	
	on nextautolink pressed do
	(
		Windows.sendMessage addlink.hwnd[1] 0x0201 0 0 --发送左键按下消息
		Windows.sendMessage addlink.hwnd[1] 0x0202 0 0 --发送左键抬起消息
		try
		(
			if tvdst.SelectedNode.NextVisibleNode!=undefined  and tvsrc.SelectedNode.NextVisibleNode!=undefined then
			(
				tvdst.SelectedNode=tvdst.SelectedNode.NextVisibleNode
				tvsrc.SelectedNode=tvsrc.SelectedNode.NextVisibleNode
			)
		)
		catch()
	)
	
	on refresh pressed do
	(
		try
		(
			obj=tvdst.Nodes.Item[0].tag.value[2]
		)
		catch()
		if isValidNode obj do initTV tvdst obj
	)
	
	on showabout pressed do
	(
		format "%s\n" "pressed"
		createDialog about_rollout modal:true
	)
)

createDialog Anim_toolbox_rollout width:550 height:460
