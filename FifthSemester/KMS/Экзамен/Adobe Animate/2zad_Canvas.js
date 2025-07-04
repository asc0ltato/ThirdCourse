(function (lib, img, cjs, ss) {

var p; // shortcut to reference prototypes

// library properties:
lib.properties = {
	width: 550,
	height: 400,
	fps: 24,
	color: "#FFFFFF",
	opacity: 1.00,
	manifest: []
};



lib.ssMetadata = [];


// symbols:



(lib.button3 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f().s("#000000").ss(1,1,1).p("AlsjqILZAAIAAHVIrZAAg");
	this.shape.setTransform(36.5,23.5);

	this.shape_1 = new cjs.Shape();
	this.shape_1.graphics.f("#0066CC").s().p("AlsDqIAAnUILYAAIAAHUg");
	this.shape_1.setTransform(36.5,23.5);

	this.timeline.addTween(cjs.Tween.get({}).to({state:[{t:this.shape_1},{t:this.shape}]}).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = new cjs.Rectangle(-1,-1,75,49);


(lib.button2 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f().s("#000000").ss(1,1,1).p("AmdjlIM7AAIAAHLIs7AAg");
	this.shape.setTransform(41.5,23);

	this.shape_1 = new cjs.Shape();
	this.shape_1.graphics.f("#0066CC").s().p("AmdDlIAAnKIM7AAIAAHKg");
	this.shape_1.setTransform(41.5,23);

	this.timeline.addTween(cjs.Tween.get({}).to({state:[{t:this.shape_1},{t:this.shape}]}).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = new cjs.Rectangle(-1,-1,85,48);


(lib.button1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f().s("#000000").ss(1,1,1).p("AmAjWIMBAAIAAGtIsBAAg");
	this.shape.setTransform(38.5,21.5);

	this.shape_1 = new cjs.Shape();
	this.shape_1.graphics.f("#0066CC").s().p("AmADXIAAmtIMBAAIAAGtg");
	this.shape_1.setTransform(38.5,21.5);

	this.timeline.addTween(cjs.Tween.get({}).to({state:[{t:this.shape_1},{t:this.shape}]}).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = new cjs.Rectangle(-1,-1,79,45);


(lib.Символ1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f().s("#000000").ss(1,1,1).p("AGGAAQAABuhzBNQhyBOihAAQigAAhzhOQhyhNAAhuQAAhtByhNQBzhOCgAAQChAAByBOQBzBNAABtg");
	this.shape.setTransform(39,26.5);

	this.shape_1 = new cjs.Shape();
	this.shape_1.graphics.f("#0066CC").s().p("AkSC7QhzhNAAhuQAAhsBzhOQByhOCgAAQChAAByBOQBzBOAABsQAABuhzBNQhyBOihAAQigAAhyhOg");
	this.shape_1.setTransform(39,26.5);

	this.timeline.addTween(cjs.Tween.get({}).to({state:[{t:this.shape_1},{t:this.shape}]}).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = new cjs.Rectangle(-1,-1,80,55);


// stage content:
(lib._2zad_Canvas = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_0 = function() {
		this.stop();
		
		this.button1.addEventListener("click", f1.bind(this));
		function f1(args)
		{this.play();}
		
		this button2.addEventListener("click", f2.bind(this));
		function f2(args)
		{this.stop();}
		
		this button3.addEventListener("click", f3.bind(this));
		function f3(args)
		{this.gotoAndPlay();}
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).call(this.frame_0).wait(60));

	// Слой 3
	this.button3 = new lib.button3();
	this.button3.parent = this;
	this.button3.setTransform(492.5,39.6,1,1,0,0,0,36.5,23.5);
	new cjs.ButtonHelper(this.button3, 0, 1, 1);

	this.button2 = new lib.button2();
	this.button2.parent = this;
	this.button2.setTransform(382.6,41.1,1,1,0,0,0,41.5,23);
	new cjs.ButtonHelper(this.button2, 0, 1, 1);

	this.button1 = new lib.button1();
	this.button1.parent = this;
	this.button1.setTransform(264.6,38.6,1,1,0,0,0,38.5,21.5);
	new cjs.ButtonHelper(this.button1, 0, 1, 1);

	this.timeline.addTween(cjs.Tween.get({}).to({state:[{t:this.button1},{t:this.button2},{t:this.button3}]}).wait(60));

	// Слой 1
	this.instance = new lib.Символ1("synched",0);
	this.instance.parent = this;
	this.instance.setTransform(64.2,86.1,1,1,15,0,0,39.1,26.5);

	this.timeline.addTween(cjs.Tween.get(this.instance).wait(1).to({regX:39,rotation:14.8,x:75.1,y:86},0).wait(1).to({rotation:29,x:85.4,y:88.2},0).wait(1).to({rotation:28.7,x:101.7,y:92.4},0).wait(1).to({rotation:28.3,x:124.2,y:98.1},0).wait(1).to({rotation:28.1,x:135.3,y:100.9},0).wait(1).to({rotation:27.9,x:146.2,y:103.7},0).wait(1).to({rotation:27.7,x:158.4,y:106.8},0).wait(1).to({rotation:13.1,x:172.9,y:108},0).wait(1).to({rotation:127.3,x:172.4,y:118.1},0).wait(1).to({rotation:127,x:166.4,y:131.6},0).wait(1).to({rotation:126.8,x:161,y:143.3},0).wait(1).to({rotation:126.7,x:156.7,y:152.9},0).wait(1).to({rotation:126.5,x:153.7,y:159.5},0).wait(1).to({rotation:12,x:152.8,y:163},0).wait(1).to({rotation:11.9,x:157.3,y:163.1},0).wait(1).to({rotation:11.8,x:162.1,y:163},0).wait(1).to({x:167.5},0).wait(1).to({rotation:11.7,x:173.7,y:163.1},0).wait(1).to({rotation:11.5,x:181.2,y:163},0).wait(1).to({rotation:11.4,x:191.1,y:163.1},0).wait(1).to({rotation:11.1,x:203.8},0).wait(1).to({rotation:10.9,x:215.5,y:163},0).wait(1).to({rotation:10.8,x:225.4,y:163.1},0).wait(1).to({rotation:10.6,x:233.5,y:163},0).wait(1).to({rotation:10.5,x:240.6},0).wait(1).to({rotation:10.4,x:248.8},0).wait(1).to({rotation:60.9,x:259.5,y:163.6},0).wait(1).to({rotation:60.7,x:268,y:173.9},0).wait(1).to({rotation:122.1,x:266.1,y:188.8},0).wait(1).to({rotation:121.8,x:260.2,y:203.1},0).wait(1).to({rotation:121.6,x:256,y:213.4},0).wait(1).to({rotation:121.5,x:253.1,y:220.4},0).wait(1).to({rotation:121.4,x:251.2,y:225},0).wait(1).to({x:249.9,y:228},0).wait(1).to({rotation:121.3,x:249.1,y:230.1},0).wait(1).to({x:248.3,y:232.1},0).wait(1).to({x:247.3,y:234.5},0).wait(1).to({rotation:15.2,x:250.5,y:235.4},0).wait(1).to({rotation:15.1,x:256.8,y:236.1},0).wait(1).to({rotation:14.9,x:266.3,y:237.2},0).wait(1).to({rotation:14.7,x:280.4,y:238.8},0).wait(1).to({rotation:14.4,x:300.7,y:241},0).wait(1).to({rotation:13.9,x:327.9,y:244.1},0).wait(1).to({rotation:7.1,x:350.9,y:246},0).wait(1).to({rotation:44.4,x:367,y:252.2},0).wait(1).to({rotation:118.6,x:368.7,y:264.1},0).wait(1).to({rotation:118.4,x:363.7,y:276.6},0).wait(1).to({rotation:118.2,x:358.7,y:289},0).wait(1).to({rotation:117.9,x:353.2,y:302.3},0).wait(1).to({rotation:-1.9,x:350.8,y:314.7},0).wait(1).to({rotation:-2.2,x:369.5,y:312.3},0).wait(1).to({rotation:-2.5,x:389.7,y:309.6},0).wait(1).to({rotation:-2.9,x:410.4,y:306.9},0).wait(1).to({rotation:-3.3,x:431.3,y:304.2},0).wait(1).to({rotation:-3.6,x:452,y:301.5},0).wait(1).to({rotation:37.2,x:470.8,y:305.2},0).wait(1).to({rotation:105.6,x:468.1,y:323.8},0).wait(1).to({rotation:105.3,x:464.3,y:341.4},0).wait(1).to({regX:39.1,regY:26.4,rotation:105,x:461.1,y:356.2},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = new cjs.Rectangle(299.7,215.1,505.3,99.7);

})(lib = lib||{}, images = images||{}, createjs = createjs||{}, ss = ss||{});
var lib, images, createjs, ss;