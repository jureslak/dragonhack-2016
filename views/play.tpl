%rebase base
<div id='all'>
<form action='/runapp/' method="post">
  <label>Minimal length:</label>
  <input type='text' name='mini' value='3'><br>
  <label>Maximal length:</label>
  <input type='text' name='maxi' value='5'><br>
  <input type="button" name='done' value='run' class='btn btn-default'
         id='runbutton' onclick="run_test()">
</form>

<div rv-if="glob.problem" id="prob">
  <h1>Current problem</h1>
  <img class='state' rv-src='glob.problem.start | img'>
  to
  <img class='state' rv-src='glob.problem.end | img'>
</div>

<div rv-if='glob.result' id='raw-results' class="collapse in">
  <h1>Last test results</h1>
  <table class="table table-bordered" id="result-table">
    <tr rv-each-row="glob.result">
      <td rv-each-data="row">{data}</td>
    </tr>
  </table>

  <div>Your moves: {glob.result | len}</div>
  <div>Optimal moves: {glob.problem.path | len}</div>
  <h3>Show optimal vs. your path:</h3><a href='#' class="btn btn-primary btn-lg" onclick='$("optpath").toggle();'>toggle</a>
  <div id='optpath'>
    <ul>
      <li rv-each-state="glob.problem.path" class='pathdesc'>
        <img class='state' rv-src='state | img'>
      </li>
    </ul>
  </div>
  -------------------------
  <div id='yourpath'>
    <ul>
      <li rv-each-state="glob.your_path" class='pathdesc'>
        <img class='state' rv-src='state | img'>
      </li>
    </ul>
  </div>
  <h3>View the statistics</h3>
  link to stats
</div>

<div rv-unless="glob.result">No results.</span>


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
  $.post('/runapp/', {'mini': 3, 'maxi': 5}, function(data) {
    glob.problem = $.parseJSON(data);
  });
  setTimeout(has_ended, 1000);
}
function finished() {
  $.post('/get_result/', {}, function(data) {
    var result = $.parseJSON(data);
    glob.result = result.moves;
    glob.your_path = result.your_path;
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

});
</script>
