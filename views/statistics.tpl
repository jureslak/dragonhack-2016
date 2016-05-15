% rebase base
% import json
<div class="row">
  <div class="col-md-8 col-md-offset-2">
    <h1>Statistics</h1>
    <p>
      Tests taken: {{games_played}} <br />
      Optimal solutions found: {{optimal_games_played}} <br />
      Average optimal solution length: {{avg_opt_sol_len}} <br>
      Your optimal solution length: {{avg_usr_sol_len}}
    </p>

    <h3>Your vs. optimal moves</h3>
    <div id="chart-container1" class="chart-container">
      <div id="y-axis1" class="y-axis"></div>
      <div id="chart1" class="chart"></div>
      <div id="legend1" class="legend"></div>
    </div>

    <h3>Shaking levels</h3>
    <div id="chart-container2" class="chart-container">
      <div id="y-axis2" class="y-axis"></div>
      <div id="chart2" class="chart"></div>
      <div id="legend2" class="legend"></div>
    </div>

  </div>
</div>

<script src="/static/rickshaw/vendor/d3.min.js"></script>
<script src="/static/rickshaw/vendor/d3.layout.min.js"></script>
<script src="/static/rickshaw/rickshaw.min.js"></script>
<script>
var graph1 = new Rickshaw.Graph({
    element: document.querySelector("#chart1"),
    renderer: 'line',
    series: [{
            name: "Your number of moves",
            data: {{!json.dumps(usr_moves)}},
            color: 'steelblue'
    }, {
            name: "Optimal number of moves",
            data: {{!json.dumps(opt_moves)}},
            color: 'lightblue'
    }]
});
var x_axis1 = new Rickshaw.Graph.Axis.X( { graph: graph1 } );
var y_axis1 = new Rickshaw.Graph.Axis.Y( {
        graph: graph1,
        orientation: 'left',
        //tickFormat: Rickshaw.Fixtures.Number.formatKMBT,
        element: document.getElementById('y-axis1'),
} );

graph1.render();

var legend1 = new Rickshaw.Graph.Legend({
    graph: graph1,
    element: document.querySelector('#legend1')
});

var hoverDetail1 = new Rickshaw.Graph.HoverDetail( {
    graph: graph1
} );

var graph2 = new Rickshaw.Graph({
    element: document.querySelector("#chart2"),
    renderer: 'line',
    series: [{
            name: "Shaking level",
            data: {{!json.dumps(acc)}},
            color: 'steelblue'
    }]
});
var x_axis2 = new Rickshaw.Graph.Axis.X( { graph: graph2 } );
var y_axis2 = new Rickshaw.Graph.Axis.Y( {
        graph: graph2,
        orientation: 'left',
        //tickFormat: Rickshaw.Fixtures.Number.formatKMBT,
        element: document.getElementById('y-axis2'),
} );

graph2.render();

var legend2 = new Rickshaw.Graph.Legend({
    graph: graph2,
    element: document.querySelector('#legend2')
});

var hoverDetail2 = new Rickshaw.Graph.HoverDetail( {
    graph: graph2
} );

</script>
