(function (lib, img, cjs, ss) {

var p; // shortcut to reference prototypes

// library properties:
lib.properties = {
	width: 550,
	height: 400,
	fps: 24,
	color: "#FFFFFF",
	opacity: 1.00,
	manifest: [
		{src:"images/cms.jpg?1726841471299", id:"cms"},
		{src:"sounds/amper.mp3?1726841471299", id:"amper"},
		{src:"sounds/micro.mp3?1726841471299", id:"micro"},
		{src:"sounds/nagrev.mp3?1726841471299", id:"nagrev"},
		{src:"sounds/peso.mp3?1726841471299", id:"peso"},
		{src:"sounds/pito.mp3?1726841471299", id:"pito"},
		{src:"sounds/tempa.mp3?1726841471299", id:"tempa"},
		{src:"sounds/terma.mp3?1726841471299", id:"terma"},
		{src:"sounds/truba.mp3?1726841471299", id:"truba"},
		{src:"sounds/ventil.mp3?1726841471299", id:"ventil"},
		{src:"sounds/volt.mp3?1726841471299", id:"volt"}
	]
};



lib.ssMetadata = [];


// symbols:



(lib.cms = function() {
	this.initialize(img.cms);
}).prototype = p = new cjs.Bitmap();
p.nominalBounds = new cjs.Rectangle(0,0,3200,2400);


(lib.volt_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("volt");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("AkqEiIAApDIJVAAIAAJDg");
	this.shape.setTransform(-1.1,1.1);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.ventil_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("ventil");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("Ak/E/QiEiEAAi7QAAi6CEiFQCFiEC6AAQC7AACECEQCFCFAAC6QAAC7iFCEQiECFi7AAQi6AAiFiFg");
	this.shape.setTransform(-0.7,2.3);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.Truba = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("truba");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("A8BCuIAAlcMA4DAAAIAAFcg");
	this.shape.setTransform(7.6,2.6);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.terma_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("terma");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("Ag7LzIAA3kIB3AAIAAXkg");
	this.shape.setTransform(0,-2.4);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.tempa_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("tempa");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("Ak6DbIAAm2IJ0AAIAAG2g");
	this.shape.setTransform(-0.5,1.1);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.pito_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("pito");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("AjvCgIAAk/IHfAAIAAE/g");
	this.shape.setTransform(0,2.1);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.peso_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("peso");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("AkWEYIAAouIIuAAIAAIug");
	this.shape.setTransform(-1,2.1);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.nagrev_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("nagrev");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("AlOCfIAAk9IKdAAIAAE9g");
	this.shape.setTransform(-0.5,1.1);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.micro_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("micro");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("Ak6DcIAAm3IJ0AAIAAG3g");
	this.shape.setTransform(-0.5,-0.9);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


(lib.amper_1 = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// timeline functions:
	this.frame_2 = function() {
		playSound("amper");
	}

	// actions tween:
	this.timeline.addTween(cjs.Tween.get(this).wait(2).call(this.frame_2).wait(2));

	// Слой 1
	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("AlmEOIAAoaILOAAIAAIag");
	this.shape.setTransform(-1,0.1);
	this.shape._off = true;

	this.timeline.addTween(cjs.Tween.get(this.shape).wait(3).to({_off:false},0).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = null;


// stage content:



(lib.Картаустановки_Canvas = function(mode,startPosition,loop) {
	this.initialize(mode,startPosition,loop,{});

	// Слой 1
	this.instance = new lib.amper_1();
	this.instance.parent = this;
	this.instance.setTransform(163.9,193.2,0.9,1.019,0,0,0,0.1,0);
	new cjs.ButtonHelper(this.instance, 0, 1, 2, false, new lib.amper_1(), 3);

	this.instance_1 = new lib.peso_1();
	this.instance_1.parent = this;
	this.instance_1.setTransform(154.2,293.9,0.816,0.614);
	new cjs.ButtonHelper(this.instance_1, 0, 1, 2, false, new lib.peso_1(), 3);

	this.instance_2 = new lib.pito_1();
	this.instance_2.parent = this;
	this.instance_2.setTransform(43.5,310.2,0.95,1.147);
	new cjs.ButtonHelper(this.instance_2, 0, 1, 2, false, new lib.pito_1(), 3);

	this.instance_3 = new lib.micro_1();
	this.instance_3.parent = this;
	this.instance_3.setTransform(148.2,332.1,3.97,2.358,0,0,0,0.1,0);
	new cjs.ButtonHelper(this.instance_3, 0, 1, 2, false, new lib.micro_1(), 3);

	this.instance_4 = new lib.nagrev_1();
	this.instance_4.parent = this;
	this.instance_4.setTransform(224,251,0.659,0.528,0,0,0,0,0.1);
	new cjs.ButtonHelper(this.instance_4, 0, 1, 2, false, new lib.nagrev_1(), 3);

	this.instance_5 = new lib.tempa_1();
	this.instance_5.parent = this;
	this.instance_5.setTransform(379.3,184.5,1.089,1.53);
	new cjs.ButtonHelper(this.instance_5, 0, 1, 2, false, new lib.tempa_1(), 3);

	this.instance_6 = new lib.terma_1();
	this.instance_6.parent = this;
	this.instance_6.setTransform(338.5,194.8,1,0.661);
	new cjs.ButtonHelper(this.instance_6, 0, 1, 2, false, new lib.terma_1(), 3);

	this.instance_7 = new lib.terma_1();
	this.instance_7.parent = this;
	this.instance_7.setTransform(70,191,1,0.713);
	new cjs.ButtonHelper(this.instance_7, 0, 1, 2, false, new lib.terma_1(), 3);

	this.instance_8 = new lib.ventil_1();
	this.instance_8.parent = this;
	this.instance_8.setTransform(487,255.2,0.857,0.867,0,0,0,0,0.1);
	new cjs.ButtonHelper(this.instance_8, 0, 1, 2, false, new lib.ventil_1(), 3);

	this.instance_9 = new lib.volt_1();
	this.instance_9.parent = this;
	this.instance_9.setTransform(236.5,195.6,1.026,0.918,0,0,0,-0.8,1.4);
	new cjs.ButtonHelper(this.instance_9, 0, 1, 2, false, new lib.volt_1(), 3);

	this.instance_10 = new lib.Truba();
	this.instance_10.parent = this;
	this.instance_10.setTransform(200.7,257.9);
	new cjs.ButtonHelper(this.instance_10, 0, 1, 2, false, new lib.Truba(), 3);

	this.instance_11 = new lib.cms();
	this.instance_11.parent = this;
	this.instance_11.setTransform(0,0,0.174,0.17);

	this.shape = new cjs.Shape();
	this.shape.graphics.f("#0066CC").s().p("AgdBSIAAijIA7AAIAACjg");
	this.shape.setTransform(315.5,323.6);

	this.timeline.addTween(cjs.Tween.get({}).to({state:[{t:this.shape},{t:this.instance_11},{t:this.instance_10},{t:this.instance_9},{t:this.instance_8},{t:this.instance_7},{t:this.instance_6},{t:this.instance_5},{t:this.instance_4},{t:this.instance_3},{t:this.instance_2},{t:this.instance_1},{t:this.instance}]}).wait(1));

}).prototype = p = new cjs.MovieClip();
p.nominalBounds = new cjs.Rectangle(275,200,557.4,408.6);

})(lib = lib||{}, images = images||{}, createjs = createjs||{}, ss = ss||{});
var lib, images, createjs, ss;