%rebase base
<div id='all'>
<h1>Choose difficulty</h1>
<form action='/runapp/' method="post">
  <table>
    <tr>
      <td><label>Minimal number of optimal moves:</label></td>
      <td><input type='text' name='mini' value='3' id='mini'></td>
    </tr>
    <tr>
      <td><label>Maximal number of optimal moves:</label></td>
      <td><input type='text' name='maxi' value='5' id='maxi'></td>
    </tr>
  </table>
  <input type="button" name='done' value='Run' class='btn btn-primary' id='runbutton' onclick="run_test()">
</form>

<div rv-if="glob.problem" id="prob">
  <h1>Current problem</h1>
  <img class='state' rv-src='glob.problem.start | img'>
  <img class='arrow' src='/static/img/arrow.png'>
  <img class='state' rv-src='glob.problem.end | img'>
</div>

<div rv-if='glob.result' id='raw-results' class="collapse in">
  <h1>Last test results</h1>
  <table class="table table-striped table-hover" id="result-table">
    <tr rv-each-row="glob.result">
      <td rv-each-data="row">{data}</td>
    </tr>
  </table>
  <h4>Shaking level: {glob.acc}</h4>
  <h4>Your moves: {glob.result | lenm1}</h4>
  <h4>Optimal moves: {glob.problem.path | lenm1}</h4>
  <h3>Show optimal path vs. your path</h3>
  <button class="btn btn-primary btn-md" data-toggle="collapse" data-target="#paths" onclick='arrhide()'>Toggle</button>
  <div id="paths" class="collapse">
    <h4>Optimal moves</h4>
    <div id='optpath' class='path'>
      <ul>
        <li rv-each-state="glob.problem.path" class='pathdesc'>
          <img class='state' rv-src='state | img'>
          <img class='arrow' src='/static/img/arrow.png'>
        </li>
      </ul>
    </div>
    <h4>Your moves</h4>
    <div id='yourpath' class='path'>
      <ul>
        <li rv-each-state="glob.your_path" class='pathdesc'>
          <img class='state' rv-src='state | img'>
          <img class='arrow' src='/static/img/arrow.png'>
        </li>
      </ul>
    </div>
  </div>
  <h3><a href="/statistics/">View the statistics</a></h3>
</div>

<div rv-unless="glob.result">
<p id="no-result">No results.</p>
</div>

</div>
<!-- <script src="/static/js/rivets.js"></script> -->
<script>
function has_ended() {
  $.post('/is_running/', {}, function(data) {
    console.log("here");
    if (data == "true") {
      setTimeout(has_ended, 1000);
    } else {
      finished();
    }
  });
}
var glob = {};
function run_test() {
  glob.running = true;
  glob.result = undefined;
  $("#runbutton").attr("disabled", true);
  var mini = parseInt($("#mini").val());
  var maxi = parseInt($("#maxi").val());
  $.post('/runapp/', {'mini': mini, 'maxi': maxi}, function(data) {
    glob.problem = $.parseJSON(data);
  });
  setTimeout(has_ended, 1000);
}
function finished() {
  $.post('/get_result/', {}, function(data) {
    var result = $.parseJSON(data);
    if (result.fail == false) {
      glob.result = result.moves;
      glob.your_path = result.your_path;
      glob.acc = result.acc;
    }
  });
  $("#runbutton").attr("disabled", false);
  glob.running = false;

}
$(function() {
rivets.configure({

  // Attribute prefix in templates
  prefix: 'rv',

  // Preload templates with initial data on bind
  preloadData: true,

  // Root sightglass interface for keypaths
  rootInterface: '.',

  // Template delimiters for text bindings
  templateDelimiters: ['{', '}'],

  // Augment the event handler of the on-* binder
  handler: function(target, event, binding) {
    this.call(target, event, binding.view.models)
  }
});

rivets.bind(document.querySelector('#all'), {
  glob: glob
});

rivets.formatters.img = function(value){
  return '/static/img/'+value.replace(/\|/g, "I")+'.png';
}

rivets.formatters.len = function(value){
  return value.length;
}

rivets.formatters.lenm1 = function(value){
  return value.length - 1;
}
});
function arrhide() {
  var a = $('#optpath img.arrow');
  a[a.length-1].style.display = 'none';
  var b = $('#yourpath img.arrow');
  b[b.length-1].style.display = 'none';
}
</script>
